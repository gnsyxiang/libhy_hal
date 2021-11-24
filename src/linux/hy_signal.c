/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_signal.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    25/10 2021 19:03
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        25/10 2021      create the file
 * 
 *     last modified: 25/10 2021 19:03
 */
#include <stdio.h>

#include "hy_signal.h"

#if (__linux__ && __GLIBC__ && !__UCLIBC__) || __APPLE__

#include <sys/types.h>
#include <unistd.h>
#include <execinfo.h>
#include <stdlib.h>
#include <string.h>

#include "hy_mem.h"
#include "hy_type.h"
#include "hy_string.h"
#include "hy_assert.h"
#include "hy_log.h"

#define ALONE_DEBUG 1

#define BACKTRACE_SIZE   32

#define _ADD_SIGNAL(sig, act)                               \
    do {                                                    \
        for (int i = 0; i < HY_SIGNAL_NUM_MAX_32; ++i) {    \
            if (sig[i] == 0) {                              \
                continue;                                   \
            }                                               \
            sigaction(sig[i], act, NULL);                   \
        }                                                   \
    } while (0);

typedef struct {
    HySignalSaveConfig_t    save_config;
} _signal_context_t;

static _signal_context_t *context = NULL;

static char *signal_str[] = {
    [1] =  "SIGHUP",      [2] =  "SIGINT",      [3] =  "SIGQUIT",     [4] =  "SIGILL",
    [5] =  "SIGTRAP",     [6] =  "SIGABRT",     [7] =  "",            [8] =  "SIGFPE",
    [9] =  "SIGKILL",     [10] = "SIGUBS",      [11] = "SIGSEGV",     [12] = "SIGSYS",
    [13] = "SIGPIPE",     [14] = "SIGALRM",     [15] = "SIGTERM",     [16] = "SIGUSR1",
    [17] = "SIGUSR2",     [18] = "SIGCHLD",     [19] = "SIGPWR",      [20] = "SIGWINCH",
    [21] = "SIGURG",      [22] = "SIGPOLL",     [23] = "SIGSTOP",     [24] = "SIGTSTP",
    [25] = "SIGCONT",     [26] = "SIGTTIN",     [27] = "SIGTTOU",     [28] = "SIGVTALRM",
    [29] = "SIGPROF",     [30] = "SIGXCPU",     [31] = "SIGXFSZ",
};

static void _dump_backtrace(void)
{
    int nptrs;
    char **strings = NULL;
    void *buffer[BACKTRACE_SIZE];

    nptrs = backtrace(buffer, BACKTRACE_SIZE);

    printf("Call Trace:\n");
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("Not Found\n\n");
        return ;
    }

    for (int j = 0; j < nptrs; j++) {
        printf("  [%02d] %s\n", j, strings[j]);
    }

    HY_MEM_FREE_P(strings);
}

static void _error_handler(int signo)
{
    HySignalSaveConfig_t *save_config = &context->save_config;

    if (save_config->error_cb) {
        save_config->error_cb(save_config->args);
    }

    LOGE("\n\n <<<%s(%d)>>> crashed by signal %s \n",
            save_config->app_name, getpid(), signal_str[signo]);

    _dump_backtrace();

    if (signo == SIGINT || signo == SIGUSR1 || signo == SIGUSR2) {
        exit(-1);
    } else {
        char cmd[256] = {0};
        sprintf(cmd, "mkdir -p %s", save_config->coredump_path);
        system(cmd);
        sprintf(cmd, "cat /proc/%d/maps", getpid());
        printf("Process maps:\n");
        system(cmd);

        snprintf(cmd, 256, "cat /proc/%d/maps > %s/%s.%d.maps",
             getpid(), save_config->coredump_path, save_config->app_name, getpid());
        system(cmd);
    }
}

static void _user_handler(int signo)
{
    HySignalSaveConfig_t *save_config = &context->save_config;

    if (save_config->user_cb) {
        save_config->user_cb(save_config->args);
    }
}

void HySignalDestroy(void **handle)
{
    HY_MEM_FREE_PP(&context);

    LOGI("signal destroy successful \n");
}

void *HySignalCreate(HySignalConfig_t *config)
{
    HY_ASSERT_VAL_RET_VAL(!config, NULL);

    struct sigaction act;

    do {
        context = HY_MEM_MALLOC_BREAK(_signal_context_t *, sizeof(*context));
        HY_MEMCPY(&context->save_config, &config->save_config, sizeof(config->save_config));

        act.sa_flags = SA_RESETHAND;
        sigemptyset(&act.sa_mask);
        act.sa_handler = _error_handler;

        _ADD_SIGNAL(config->error_num, &act);

        act.sa_handler = _user_handler;

        _ADD_SIGNAL(config->user_num, &act);

        signal(SIGPIPE, SIG_IGN);

        LOGI("signal create successful \n");
        return context;
    } while (0);

    HySignalDestroy((void **)&context);
    return NULL;
}
#else
void *HySignalCreate(HySignalConfig_t *signal_config) {return NULL;}
void HySignalDestroy(void **handle) {}
#endif

