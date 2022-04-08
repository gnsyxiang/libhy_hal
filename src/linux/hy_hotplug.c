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

#include "hy_mem.h"
#include "hy_string.h"
#include "hy_assert.h"
#include "hy_log.h"
#include "hy_thread.h"
#include "hy_hal_utils.h"

#include "hy_hotplug.h"

#define _HOTPLUG_BUF_LEN_MAX    (4 * 1024)

typedef struct {
    char                    *name;
    HyHotplugType_e         type;
} _hotplug_name_s;

typedef struct {
    HyHotplugSaveConfig_s   save_c;

    hy_s32_t                fd;
    char                    *buf;

    void                    *thread_h;
    hy_s32_t                exit_flag;
} _hotplug_context_s;

static hy_s32_t _hotplug_thread_cb(void *args)
{
    _hotplug_context_s *context = args;
    HyHotplugType_e type;
    char *seek;

    _hotplug_name_s hotplug_name[] = {
        {"mmcblk",      HY_HOTPLUG_TYPE_SDCARD},
        {"usb",         HY_HOTPLUG_TYPE_UDISK},
    };

    while (!context->exit_flag) {
_RECV:
        HY_MEMSET(context->buf, _HOTPLUG_BUF_LEN_MAX);
        if (-1 == recv(context->fd, context->buf, _HOTPLUG_BUF_LEN_MAX, 0)) {
            LOGE("recv failed \n");
            break;
        }

        LOGE("-1-buf: %s \n", context->buf);

        for (hy_u32_t i = 0; i < HyHalUtilsArrayCnt(hotplug_name); ++i) {
            seek = strstr(context->buf, hotplug_name[i].name);
            if (seek) {
                type = hotplug_name[i].type;
                LOGE("type: %s \n", hotplug_name[i].name);
                goto _RECV;
            }
        }

        seek = strstr(context->buf, "unbind");
        if (seek) {
            LOGE("unbind\n");
            continue;
        }

        seek = strstr(context->buf, "bind");
        if (seek) {
            LOGE("bind\n");
            continue;
        }
    }

    return -1;
}

void HyHotplugDestroy(void **handle)
{
    LOGT("&handle: %p, handle: %p \n", handle, *handle);
    HY_ASSERT_RET(!handle || !*handle);

    _hotplug_context_s *context = *handle;

    context->exit_flag = 1;
    send(context->fd, context, sizeof(*context), 0);
    HyThreadDestroy(&context->thread_h);

    LOGI("hotplug destroy, context: %p \n", context);

    HY_MEM_FREE_PP(&context->buf);
    HY_MEM_FREE_PP(handle);
}

void *HyHotplugCreate(HyHotplugConfig_s *hotplug_c)
{
    LOGT("hotplug_c: %p \n", hotplug_c);
    HY_ASSERT_RET_VAL(!hotplug_c, NULL);

    _hotplug_context_s *context = NULL;
	struct sockaddr_nl snl;
    hy_s32_t len = _HOTPLUG_BUF_LEN_MAX;
    hy_s32_t ret = 0;

    do {
        context = HY_MEM_MALLOC_BREAK(_hotplug_context_s *, sizeof(*context));
        HY_MEMCPY(&context->save_c, &hotplug_c->save_c, sizeof(context->save_c));

        context->buf = HY_MEM_MALLOC_BREAK(char *, _HOTPLUG_BUF_LEN_MAX);

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

        memset(&snl, 0, sizeof(struct sockaddr_nl));
        snl.nl_family = AF_NETLINK;
        snl.nl_pid = getpid();
        snl.nl_groups = 1;
        ret = bind(context->fd, (struct sockaddr *)&snl, sizeof(struct sockaddr_nl));
        if (-1 == ret) {
            LOGE("bind failed \n");
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
    return NULL;
}

