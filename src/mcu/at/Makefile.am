# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    08/11 2021 16:01
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        08/11 2021      create the file
# 
#     last modified: 08/11 2021 16:01
# ===============================================================

if COMPILE_SELECT_MCU_AT32F4XX
_mcu_type = at32f4xx
endif

SUBDIRS = $(_mcu_type)

noinst_LTLIBRARIES = libmcu_vender.la

# ------------------
# defining variables
# ------------------
_no_error_com   =
_include_com    =
_sources_com    =
_cppflags_com   =
_cflags_com     =
_cxxflags_com   =
_ldflags_com    =
_ldadd_com      =

# ---------------------
# add necessary content
# ---------------------
_no_error_com   +=

_include_com    += \
                   -I$(top_srcdir)/include/mcu  \
                   -I$(top_srcdir)/include

if COMPILE_LIBHY_MCU
_include_com    += $(HY_MCU_INCS)
endif

_sources_com    +=

_cppflags_com   += $(_no_error_com) $(_include_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += $(top_builddir)/src/mcu/at/$(_mcu_type)/libmcu.la

libmcu_vender_la_SOURCES    = $(_sources_com)
libmcu_vender_la_CPPFLAGS   = $(_cppflags_com)
libmcu_vender_la_CFLAGS     = $(_cflags_com)
libmcu_vender_la_CXXFLAGS   = $(_cxxflags_com)
libmcu_vender_la_LDFLAGS    = $(_ldflags_com)
libmcu_vender_la_LIBADD     = $(_ldadd_com)

