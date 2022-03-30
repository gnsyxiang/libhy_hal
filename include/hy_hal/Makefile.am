# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    30/03 2022 09:38
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        30/03 2022      create the file
# 
#     last modified: 30/03 2022 09:38
# ===============================================================

# ------------------
# defining variables
# ------------------
_no_install_headers_com =
_install_headers_com    =

install_includedir      = $(includedir)/@project_name@

# ---------------------
# add necessary content
# ---------------------
_no_install_headers_com +=

_install_headers_com    += \
                           hy_assert.h      \
                           hy_barrier.h     \
                           hy_compile.h     \
                           hy_error.h       \
                           hy_hal_utils.h   \
                           hy_mem.h         \
                           hy_module.h      \
                           hy_printf.h      \
                           hy_string.h      \
                           hy_type.h

# ------------------
# make install files
# ------------------
noinst_HEADERS          = $(_no_install_headers_com)
install_include_HEADERS = $(_install_headers_com)

