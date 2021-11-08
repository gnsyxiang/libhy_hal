# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    29/10 2021 20:59
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        29/10 2021      create the file
# 
#     last modified: 29/10 2021 20:59
# ===============================================================

if COMPILE_SELECT_MCU_AT
_mcu_vender = at
endif

if COMPILE_SELECT_MCU_HC
_mcu_vender = hc
endif

SUBDIRS = $(_mcu_vender)

# ------------------
# defining variables
# ------------------
_no_install_headers_com =
_install_headers_com    =

install_includedir      = $(includedir)/@project_name@

# ---------------------
# add necessary content
# ---------------------
_no_install_headers_com += \
                           gpio_2_str.h \
                           uart_2_str.h

_install_headers_com    += \
                           hy_gpio.h    \
                           hy_mem.h     \
                           hy_system.h  \
                           hy_uart.h

# ------------------
# make install files
# ------------------
noinst_HEADERS          = $(_no_install_headers_com)
install_include_HEADERS = $(_install_headers_com)

