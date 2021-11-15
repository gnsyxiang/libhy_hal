# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    15/11 2021 20:10
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        15/11 2021      create the file
# 
#     last modified: 15/11 2021 20:10
# ===============================================================

if COMPILE_SELECT_MCU_AT
_mcu_vender = at
endif

if COMPILE_SELECT_MCU_HC
_mcu_vender = hc
endif

SUBDIRS = $(_mcu_vender)
