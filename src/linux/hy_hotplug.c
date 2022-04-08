/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_hotplug.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    08/04 2022 09:41
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        08/04 2022      create the file
 * 
 *     last modified: 08/04 2022 09:41
 */
#include <stdio.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <linux/netlink.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "hy_mem.h"
#include "hy_string.h"
#include "hy_assert.h"
#include "hy_log.h"
#include "hy_thread.h"
#include "hy_hal_utils.h"

#include "hy_hotplug.h"

#define _HOTPLUG_BUF_LEN_MAX    (4 * 1024)
#define _HOTPLUG_NAME_LEN_MAX   (16)

typedef struct {
    char                    *name;
    HyHotplugType_e         type;
} _hotplug_name_s;

typedef struct {
    char                    *info;
    HyHotplugState_e        state;
} _hotplug_type_s;

typedef struct {
    HyHotplugSaveConfig_s   save_c;

    hy_s32_t                fd;
    char                    *buf;

    char                    name[_HOTPLUG_NAME_LEN_MAX];
    HyHotplugType_e         type;

    void                    *thread_h;
    hy_s32_t                exit_flag;
} _hotplug_context_s;

static hy_s32_t _parse_data(_hotplug_context_s *context)
{
    char *seek = NULL;
    char *seek_tmp = NULL;
    char *result = NULL;
    HyHotplugSaveConfig_s *save_c = &context->save_c;
    _hotplug_name_s hotplug_name[] = {
        {"mmcblk",      HY_HOTPLUG_TYPE_SDCARD},
        {"usb",         HY_HOTPLUG_TYPE_UDISK},
    };
    _hotplug_type_s hotplug_type[] = {
        {"unbind",      HY_HOTPLUG_STATE_PULL_OUT},
        {"bind",        HY_HOTPLUG_STATE_INSERT},
    };

    HY_MEMSET(context->buf, _HOTPLUG_BUF_LEN_MAX);
    if (-1 == recv(context->fd, context->buf, _HOTPLUG_BUF_LEN_MAX, 0)) {
        LOGE("recv failed \n");
        return -1;
    }

    for (hy_u32_t i = 0; i < HyHalUtilsArrayCnt(hotplug_name); ++i) {
        seek = strstr(context->buf, hotplug_name[i].name);
        if (seek) {
            result = strtok_r(seek, "/", &seek_tmp);
            if (HY_STRLEN(seek_tmp) > HY_STRLEN(result)) {
                if (HY_STRLEN(seek_tmp) > HY_STRLEN(context->name)) {
                    HY_STRNCPY(context->name, sizeof(context->name),
                            seek_tmp, HY_STRLEN(seek_tmp));
                }
            } else {
                if (HY_STRLEN(result) > HY_STRLEN(context->name)) {
                    HY_STRNCPY(context->name, sizeof(context->name),
                            result, HY_STRLEN(result));
                }
            }
            context->type = hotplug_name[i].type;
            LOGD("hotplug type: %d, name: %s \n", context->type, context->name);
            return 0;
        }
    }

    if (context->type == HY_HOTPLUG_TYPE_UNKNOW) {
        return 0;
    }

    for (hy_u32_t i = 0; i < HyHalUtilsArrayCnt(hotplug_type); ++i) {
        seek = strstr(context->buf, hotplug_type[i].info);
        if (seek) {
            if (save_c->hotplug_cb) {
                save_c->hotplug_cb(context->type, context->name,
                        hotplug_type[i].state, save_c->args);
            }

            LOGD("hotplug state: %d \n", hotplug_type[i].state);
            context->type = HY_HOTPLUG_TYPE_UNKNOW;
            HY_MEMSET(context->name, sizeof(context->name));
            break;
        }
    }

    return 0;
}

static hy_s32_t _hotplug_thread_cb(void *args)
{
    _hotplug_context_s *context = args;
    struct timeval timeout;
    fd_set read_fs;

    HY_MEMSET(&read_fs, sizeof(read_fs));
    HY_MEMSET(&timeout, sizeof(timeout));

    while (!context->exit_flag) {
        FD_ZERO(&read_fs);
        FD_SET(context->fd, &read_fs);

        timeout.tv_sec = 1;
        if (select(FD_SETSIZE, &read_fs, NULL, NULL, &timeout) < 0) {
            LOGES("select failed \n");
            break;
        }

        if (FD_ISSET(context->fd, &read_fs)) {
            if (-1 == _parse_data(context)) {
                LOGE("parse data failed \n");
                break;
            }
        }
    }

    return -1;
}

static void _scoket_destroy(_hotplug_context_s *context)
{
    close(context->fd);
}

static hy_s32_t _scoket_create(_hotplug_context_s *context)
{
    hy_s32_t len = _HOTPLUG_BUF_LEN_MAX;
    struct sockaddr_nl snl;
    hy_s32_t ret = 0;

    do {
        context->fd = socket(PF_NETLINK,
                SOCK_DGRAM | SOCK_CLOEXEC, NETLINK_KOBJECT_UEVENT);
        if (-1 == context->fd) {
            LOGES("socket failed \n");
            break;
        }

        ret = setsockopt(context->fd, SOL_SOCKET,
                SO_RCVBUFFORCE, &len, sizeof(len));
        if (-1 == ret) {
            LOGES("setsockopt failed \n");
            break;
        }

        HY_MEMSET(&snl, sizeof(snl));
        snl.nl_family   = AF_NETLINK;
        snl.nl_pid      = getpid();
        snl.nl_groups   = 1;
        ret = bind(context->fd, (struct sockaddr *)&snl,
                sizeof(struct sockaddr_nl));
        if (-1 == ret) {
            LOGE("bind failed \n");
            break;
        }

        return 0;
    } while (0);

    _scoket_destroy(context);
    return -1;
}

void HyHotplugDestroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    _hotplug_context_s *context = *handle;

    context->exit_flag = 1;
    HyThreadDestroy(&context->thread_h);

    _scoket_destroy(context);

    LOGI("hotplug destroy, context: %p \n", context);

    HY_MEM_FREE_PP(&context->buf);
    HY_MEM_FREE_PP(handle);
}

void *HyHotplugCreate(HyHotplugConfig_s *hotplug_c)
{
    LOGT("hotplug_c: %p \n", hotplug_c);
    HY_ASSERT_RET_VAL(!hotplug_c, NULL);

    _hotplug_context_s *context = NULL;

    do {
        context = HY_MEM_MALLOC_BREAK(_hotplug_context_s *, sizeof(*context));
        HY_MEMCPY(&context->save_c, &hotplug_c->save_c, sizeof(context->save_c));

        context->buf = HY_MEM_MALLOC_BREAK(char *, _HOTPLUG_BUF_LEN_MAX);

        if (0 != _scoket_create(context)) {
            LOGE("_scoket_create failed \n");
            break;
        }

        context->thread_h = HyThreadCreate_m("HY_hotplug",
                _hotplug_thread_cb, context);
        if (!context->thread_h) {
            LOGE("HyThreadCreate_m failed \n");
            break;
        }

        LOGI("hotplug create, context: %p \n", context);
        return context;
    } while (0);

    LOGE("hotplug create failed \n");
    HyHotplugDestroy((void **)&context);
    return NULL;
}

