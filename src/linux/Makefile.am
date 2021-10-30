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
_define_com     =
_ldadd_com      =

# ---------------------
# add necessary content
# ---------------------
_no_error_com   +=

_define_com     += -D_GNU_SOURCE

_include_com    +=                                  \
                   -I$(top_srcdir)/include/linux    \
                   -I$(top_srcdir)/include

_sources_com    +=                  \
                   hy_dir.c         \
                   hy_dlopen.c      \
                   hy_mem.c         \
                   hy_signal.c      \
                   hy_string.c      \
                   hy_thread.c      \
                   hy_uart.c

if COMPILE_LIBLOG4CPLUS
_sources_com    += hy_log4cplus.cpp
else
_sources_com    += hy_log.c
endif

_cppflags_com   += $(_no_error_com) $(_include_com) $(_define_com)

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

