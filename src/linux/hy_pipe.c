/**
 * 
 * Release under GPLv-3.0.
 * 
 * @file    hy_pipe.c
 * @brief   
 * @author  gnsyxiang <gnsyxiang@163.com>
 * @date    04/03 2022 08:24
 * @version v0.0.1
 * 
 * @since    note
 * @note     note
 * 
 *     change log:
 *     NO.     Author              Date            Modified
 *     00      zhenquan.qiu        04/03 2022      create the file
 * 
 *     last modified: 04/03 2022 08:24
 */
#include <stdio.h>

#include "hy_assert.h"
#include "hy_log.h"
#include "hy_mem.h"
#include "hy_string.h"
#include "hy_file.h"
#include "hy_type.h"

#include "hy_pipe.h"

typedef struct {
    HyPipeSaveConfig_s  save_c;

    hy_s32_t            pfd[2];
} _pipe_context_t;

int32_t HyPipeReadFdGet(void *pipe_h)
{
    LOGT("pipe_h: %p \n", pipe_h);
    HY_ASSERT_RET_VAL(!pipe_h, -1);

    _pipe_context_t *context = pipe_h;

    return context->pfd[0];
}

int32_t HyPipeWriteFdGet(void *pipe_h)
{
    LOGT("pipe_h: %p \n", pipe_h);
    HY_ASSERT_RET_VAL(!pipe_h, -1);

    _pipe_context_t *context = pipe_h;

    return context->pfd[1];
}

int32_t HyPipeRead(void *pipe_h, void *buf, int32_t len)
{
    LOGT("pipe_h: %p, buf: %p, len: %d \n", pipe_h, buf, len);
    HY_ASSERT(pipe_h);
    HY_ASSERT(buf);

    _pipe_context_t *context = pipe_h;

    return read(context->pfd[0], buf, len);

}

int32_t HyPipeWrite(void *pipe_h, const void *buf, int32_t len)
{
    LOGT("pipe_h: %p, buf: %p, len: %d \n", pipe_h, buf, len);
    HY_ASSERT(pipe_h);
    HY_ASSERT(buf);

    _pipe_context_t *context = pipe_h;

    return write(context->pfd[1], buf, len);
}

void HyPipeDestroy(void **pipe_h)
{
    LOGT("&pipe_h: %p, pipe_h: %p \n", pipe_h, *pipe_h);
    HY_ASSERT_RET(!pipe_h || !*pipe_h);

    _pipe_context_t *context = *pipe_h;

    LOGI("hy pipe destroy, context: %p \n", context);
    HY_MEM_FREE_PP(pipe_h);
}

void *HyPipeCreate(HyPipeConfig_s *pipe_c)
{
    LOGT("pipe_c: %p \n", pipe_c);
    HY_ASSERT_RET_VAL(!pipe_c, NULL);

    _pipe_context_t *context = NULL;
    HyFileBlockState_e state = HY_FILE_BLOCK_STATE_BLOCK;

    do {
        context = HY_MEM_MALLOC_BREAK(_pipe_context_t *, sizeof(*context));

        HY_MEMCPY(&context->save_c, &pipe_c->save_c, sizeof(context->save_c));

        if (0 != pipe(context->pfd)) {
            LOGE("pipe failed \n");
            break;
        }

        if (HY_PIPE_BLOCK_STATE_NOBLOCK == pipe_c->save_c.read_block_state) {
            state = HY_FILE_BLOCK_STATE_NOBLOCK;
        }
        if (0 != HyFileBlockStateSet(context->pfd[0], state)) {
            LOGE("hy file block state set failed \n");
            break;
        }

        LOGI("hy pipe create, context: %p \n", context);
        return context;
    } while (0);

    LOGI("hy pipe create failed \n");
    HyPipeDestroy((void **)&context);
    return NULL;
}

