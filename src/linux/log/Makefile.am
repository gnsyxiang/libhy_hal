# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    21/04 2022 14:19
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        21/04 2022      create the file
# 
#     last modified: 21/04 2022 14:19
# ===============================================================

noinst_LTLIBRARIES = libhy_hal_log.la

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

_define_com     += -D_GNU_SOURCE

_include_com    += \
                   -I$(top_srcdir)/include/$(run_os)/log        \
                   -I$(top_srcdir)/include/$(run_os)/thread     \
                   -I$(top_srcdir)/include/$(run_os)            \
                   -I$(top_srcdir)/include/hy_hal               \
                   -I$(top_srcdir)/include

_sources_com    += \
                   dynamic_array.c          \
                   epoll_helper.c           \
                   format_cb.c              \
                   hy_log.c                 \
                   log_fifo.c               \
                   log_file.c               \
                   log_socket.c             \
                   log_socket_ipc.c         \
                   process_handle_data.c    \
                   process_ipc_client.c     \
                   process_ipc_server.c     \
                   process_single.c         \
                   socket_fd_node.c         \
                   thread_specific_data.c

_cppflags_com   += \
                   $(_no_error_com)     \
                   $(_include_com)      \
                   $(_define_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += $(lib_com)

libhy_hal_log_la_SOURCES    = $(_sources_com)
libhy_hal_log_la_CPPFLAGS   = $(_cppflags_com)
libhy_hal_log_la_CFLAGS     = $(_cflags_com)
libhy_hal_log_la_CXXFLAGS   = $(_cxxflags_com)
libhy_hal_log_la_LDFLAGS    = $(_ldflags_com)
libhy_hal_log_la_LIBADD     = $(_ldadd_com)

