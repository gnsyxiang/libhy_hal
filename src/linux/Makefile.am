# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    25/10 2021 18:46
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        25/10 2021      create the file
# 
#     last modified: 25/10 2021 18:46
# ===============================================================

noinst_LTLIBRARIES = libhy_hal_os.la

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
                   -I$(top_srcdir)/include/linux \
                   -I$(top_srcdir)/include

if COMPILE_LIBHY_UTILS
_include_com    += $(HY_UTILS_INCS)
endif

_sources_com    += hy_mem.c

_cppflags_com   += $(_no_error_com) $(_include_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      +=

libhy_hal_os_la_SOURCES    = $(_sources_com)
libhy_hal_os_la_CPPFLAGS   = $(_cppflags_com)
libhy_hal_os_la_CFLAGS     = $(_cflags_com)
libhy_hal_os_la_CXXFLAGS   = $(_cxxflags_com)
libhy_hal_os_la_LDFLAGS    = $(_ldflags_com)
libhy_hal_os_la_LIBADD     = $(_ldadd_com)

