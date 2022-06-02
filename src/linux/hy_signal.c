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
    HySignalSaveConfig_t    save_c;
} _signal_context_s;

static _signal_context_s *context = NULL;

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

static hy_s32_t _dump_backtrace(void)
{
    int nptrs;
    char **strings = NULL;
    void *buffer[BACKTRACE_SIZE];

    nptrs = backtrace(buffer, BACKTRACE_SIZE);
    if (nptrs <= 0) {
        LOGE("backtrace get error, nptrs: %d \n", nptrs);
        return -1;
    }

    printf("Call Trace:\n");
    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("Not Found\n\n");
        return -1;
    }

    for (int j = 0; j < nptrs; j++) {
        printf("  [%02d] %s\n", j, strings[j]);
    }

    HY_MEM_FREE_P(strings);

    return 0;
}

static void _error_handler(int signo)
{
    HySignalSaveConfig_t *save_c= &context->save_c;

    LOGE("<<<%s(pid: %d)>>> crashed by signal %s \n",
            save_c->app_name, getpid(), signal_str[signo]);

    if (save_c->error_cb) {
        save_c->error_cb(save_c->args);
    }

    if (0 != _dump_backtrace()) {
        LOGE("GCC does not support backtrace \n");
        return ;
    }

    if (signo == SIGINT || signo == SIGUSR1 || signo == SIGUSR2) {
        exit(-1);
    } else {
        char cmd[256] = {0};
        snprintf(cmd, sizeof(cmd), "mkdir -p %s", save_c->coredump_path);
        system(cmd);

        printf("Process maps:\n");
        HY_MEMSET(cmd, sizeof(cmd));
        snprintf(cmd, sizeof(cmd), "cat /proc/%d/maps", getpid());
        system(cmd);

        snprintf(cmd, 256, "cat /proc/%d/maps > %s/%s.%d.maps", getpid(),
             save_c->coredump_path, save_c->app_name, getpid());
        system(cmd);
    }
}

static void _user_handler(int signo)
{
    HySignalSaveConfig_t *save_c= &context->save_c;

    if (save_c->user_cb) {
        save_c->user_cb(save_c->args);
    }
}

void HySignalDestroy(void)
{
    LOGI("signal context: %p destroy \n", context);

    HY_MEM_FREE_PP(&context);
}

hy_s32_t HySignalCreate(HySignalConfig_t *signal_c)
{
    LOGT("signal_c: %p \n", signal_c);
    HY_ASSERT_RET_VAL(!signal_c, -1);

    struct sigaction act;

    do {
        context = HY_MEM_MALLOC_BREAK(_signal_context_s *, sizeof(*context));
        HY_MEMCPY(&context->save_c, &signal_c->save_c, sizeof(signal_c->save_c));

        act.sa_flags = SA_RESETHAND;
        sigemptyset(&act.sa_mask);
        act.sa_handler = _error_handler;

        _ADD_SIGNAL(signal_c->error_num, &act);

        act.sa_handler = _user_handler;

        _ADD_SIGNAL(signal_c->user_num, &act);

        signal(SIGPIPE, SIG_IGN);

        LOGI("signal context: %p create \n", context);
        return 0;
    } while (0);

    HySignalDestroy();
    return -1;
}
#else
void *HySignalCreate(HySignalConfig_t *signal_c) {return NULL;}
void HySignalDestroy(void **handle) {}
#endif

#if 0
Call Trace:
  [00] ../lib/libhy_hal.so.0(+0xa3c2) [0x7f4b2a24f3c2]
  [01] ../lib/libhy_hal.so.0(+0xa65d) [0x7f4b2a24f65d]
  [02] /lib/x86_64-linux-gnu/libpthread.so.0(+0x12980) [0x7f4b2a038980]
  [03] ./hy_dir_demo(haha+0x3b) [0x55eb5823c4df]
  [04] ./hy_dir_demo(main+0x1d2) [0x55eb5823c6cb]
  [05] /lib/x86_64-linux-gnu/libc.so.6(__libc_start_main+0xe7) [0x7f4b29c56c87]
  [06] ./hy_dir_demo(_start+0x2a) [0x55eb5823bcaa]
Process maps:
55eb5823b000-55eb5823d000 r-xp 00000000 08:31 4980784                    /mnt/data/nfs/pc/pc-chip/bin/hy_dir_demo
55eb5843c000-55eb5843d000 r--p 00001000 08:31 4980784                    /mnt/data/nfs/pc/pc-chip/bin/hy_dir_demo
55eb5843d000-55eb5843e000 rw-p 00002000 08:31 4980784                    /mnt/data/nfs/pc/pc-chip/bin/hy_dir_demo
55eb5a094000-55eb5a0b5000 rw-p 00000000 00:00 0                          [heap]
7f4b24000000-7f4b24021000 rw-p 00000000 00:00 0 
7f4b24021000-7f4b28000000 ---p 00000000 00:00 0 
7f4b29018000-7f4b2902f000 r-xp 00000000 08:01 1447302                    /lib/x86_64-linux-gnu/libgcc_s.so.1
7f4b2902f000-7f4b2922e000 ---p 00017000 08:01 1447302                    /lib/x86_64-linux-gnu/libgcc_s.so.1
7f4b2922e000-7f4b2922f000 r--p 00016000 08:01 1447302                    /lib/x86_64-linux-gnu/libgcc_s.so.1
7f4b2922f000-7f4b29230000 rw-p 00017000 08:01 1447302                    /lib/x86_64-linux-gnu/libgcc_s.so.1
7f4b29230000-7f4b29231000 ---p 00000000 00:00 0 
7f4b29231000-7f4b29a31000 rw-p 00000000 00:00 0 
7f4b29a31000-7f4b29a34000 r-xp 00000000 08:01 1447322                    /lib/x86_64-linux-gnu/libdl-2.27.so
7f4b29a34000-7f4b29c33000 ---p 00003000 08:01 1447322                    /lib/x86_64-linux-gnu/libdl-2.27.so
7f4b29c33000-7f4b29c34000 r--p 00002000 08:01 1447322                    /lib/x86_64-linux-gnu/libdl-2.27.so
7f4b29c34000-7f4b29c35000 rw-p 00003000 08:01 1447322                    /lib/x86_64-linux-gnu/libdl-2.27.so
7f4b29c35000-7f4b29e1c000 r-xp 00000000 08:01 1447286                    /lib/x86_64-linux-gnu/libc-2.27.so
7f4b29e1c000-7f4b2a01c000 ---p 001e7000 08:01 1447286                    /lib/x86_64-linux-gnu/libc-2.27.so
7f4b2a01c000-7f4b2a020000 r--p 001e7000 08:01 1447286                    /lib/x86_64-linux-gnu/libc-2.27.so
7f4b2a020000-7f4b2a022000 rw-p 001eb000 08:01 1447286                    /lib/x86_64-linux-gnu/libc-2.27.so
7f4b2a022000-7f4b2a026000 rw-p 00000000 00:00 0 
7f4b2a026000-7f4b2a040000 r-xp 00000000 08:01 1451077                    /lib/x86_64-linux-gnu/libpthread-2.27.so
7f4b2a040000-7f4b2a23f000 ---p 0001a000 08:01 1451077                    /lib/x86_64-linux-gnu/libpthread-2.27.so
7f4b2a23f000-7f4b2a240000 r--p 00019000 08:01 1451077                    /lib/x86_64-linux-gnu/libpthread-2.27.so
7f4b2a240000-7f4b2a241000 rw-p 0001a000 08:01 1451077                    /lib/x86_64-linux-gnu/libpthread-2.27.so
7f4b2a241000-7f4b2a245000 rw-p 00000000 00:00 0 
7f4b2a245000-7f4b2a261000 r-xp 00000000 08:31 4980774                    /mnt/data/nfs/pc/pc-chip/lib/libhy_hal.so.0.0.7
7f4b2a261000-7f4b2a460000 ---p 0001c000 08:31 4980774                    /mnt/data/nfs/pc/pc-chip/lib/libhy_hal.so.0.0.7
7f4b2a460000-7f4b2a461000 r--p 0001b000 08:31 4980774                    /mnt/data/nfs/pc/pc-chip/lib/libhy_hal.so.0.0.7
7f4b2a461000-7f4b2a462000 rw-p 0001c000 08:31 4980774                    /mnt/data/nfs/pc/pc-chip/lib/libhy_hal.so.0.0.7
7f4b2a462000-7f4b2a48b000 r-xp 00000000 08:01 1447282                    /lib/x86_64-linux-gnu/ld-2.27.so
7f4b2a668000-7f4b2a66a000 rw-p 00000000 00:00 0 
7f4b2a689000-7f4b2a68b000 rw-p 00000000 00:00 0 
7f4b2a68b000-7f4b2a68c000 r--p 00029000 08:01 1447282                    /lib/x86_64-linux-gnu/ld-2.27.so
7f4b2a68c000-7f4b2a68d000 rw-p 0002a000 08:01 1447282                    /lib/x86_64-linux-gnu/ld-2.27.so
7f4b2a68d000-7f4b2a68e000 rw-p 00000000 00:00 0 
7ffc98b52000-7ffc98b73000 rw-p 00000000 00:00 0                          [stack]
7ffc98ba2000-7ffc98ba5000 r--p 00000000 00:00 0                          [vvar]
7ffc98ba5000-7ffc98ba6000 r-xp 00000000 00:00 0                          [vdso]
ffffffffff600000-ffffffffff601000 --xp 00000000 00:00 0                  [vsyscall]
Segmentation fault (core dumped)

addr2line 14df -e ./hy_dir_demo -f
分析：
1，addr2line需要使用相对地址，而backtrace_symbols给出的是绝对地址

2, 相对地址计算
    14df = 0x55eb5823c4df - 55eb5823b000
    其中0x55eb5823c4df是backtrace_symbols给出的绝对地址，或者是dmesg中ip输出的地址
    其中55eb5823b000是程序的base地址

3，base地址获取
    方法一，通过maps输出获取
    方法二，出现段错误时，通过dmesg输出获取

    $ dmesg | tail 
    [88850.257903] hy_dir_demo[47133]: segfault at 0 ip 000055eb5823c4df sp 00007ffc98b714f0 error 6 in hy_dir_demo[55eb5823b000+2000]
    [88850.257917] Code: 48 89 45 f8 31 c0 48 8d 35 e2 ff ff ff 48 8d 3d 6e 03 00 00 b8 00 00 00 00 e8 fd f6 ff ff 48 c7 45 f0 00 00 00 00 48 8b 45 f0 <c6> 00 01 90 48 8b 45 f8 64 48 33 04 25 28 00 00 00 74 05 e8 c9 f6
#endif

