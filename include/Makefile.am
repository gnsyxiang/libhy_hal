# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    05/03 2021 10:35
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        05/03 2021      create the file
# 
#     last modified: 05/03 2021 10:35
# ===============================================================

if COMPILE_SELECT_TARGET_OS_LINUX
select_target_os = linux
endif

if COMPILE_SELECT_TARGET_OS_MAC
select_target_os = mac
endif

if COMPILE_SELECT_TARGET_OS_WINDOW
select_target_os = window
endif

if COMPILE_SELECT_TARGET_OS_RTOS
select_target_os = rtos
endif

if COMPILE_SELECT_TARGET_OS_MCU
select_target_os = mcu
endif

SUBDIRS = $(select_target_os)

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
                           hy_hal_utils.h \
                           hy_log.h \
                           hy_module.h \
                           hy_error.h \
                           hy_compile.h \
                           hy_type.h

# ------------------
# make install files
# ------------------
noinst_HEADERS          = $(_no_install_headers_com)
install_include_HEADERS = $(_install_headers_com)

