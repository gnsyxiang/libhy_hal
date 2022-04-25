# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    24/02 2022 14:22
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        24/02 2022      create the file
# 
#     last modified: 24/02 2022 14:22
# ===============================================================

SUBDIRS = ipc_socket

noinst_LTLIBRARIES = libhy_hal_ipc.la

# ------------------
# defining variables
# ------------------
_no_error_com   =
_define_com     =
_include_com    = $(include_com)
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

_define_com     +=

_include_com    +=

_sources_com    +=

_cppflags_com   += \
                   $(_no_error_com) \
                   $(_include_com)  \
                   $(_define_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += \
                   $(top_builddir)/src/$(run_os)/ipc/ipc_socket/libhy_hal_ipc_socket.la     \
                   $(ldadd_com)

libhy_hal_ipc_la_SOURCES    = $(_sources_com)
libhy_hal_ipc_la_CPPFLAGS   = $(_cppflags_com)
libhy_hal_ipc_la_CFLAGS     = $(_cflags_com)
libhy_hal_ipc_la_CXXFLAGS   = $(_cxxflags_com)
libhy_hal_ipc_la_LDFLAGS    = $(_ldflags_com)
libhy_hal_ipc_la_LIBADD     = $(_ldadd_com)

