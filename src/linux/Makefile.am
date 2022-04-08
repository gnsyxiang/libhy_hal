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

SUBDIRS = \
          audio     \
          thread

noinst_LTLIBRARIES = libhy_hal_os.la

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
                   -I$(top_srcdir)/include/$(run_os)/thread     \
                   -I$(top_srcdir)/include/$(run_os)            \
                   -I$(top_srcdir)/include/hy_hal               \
                   -I$(top_srcdir)/include

_sources_com    += \
                   hy_dir.c         \
                   hy_dlopen.c      \
                   hy_file.c        \
                   hy_gpio.c        \
                   hy_hal_fifo.c    \
                   hy_hotplug.c     \
                   hy_log.c         \
                   hy_pipe.c        \
                   hy_sd.c          \
                   hy_signal.c      \
                   hy_socket.c      \
                   hy_storage.c     \
                   hy_time.c        \
                   hy_uart.c

_cppflags_com   += \
                   $(_no_error_com)     \
                   $(_include_com)      \
                   $(_define_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += \
                   $(top_builddir)/src/$(run_os)/thread/libhy_hal_os_thread.la  \
                   $(top_builddir)/src/$(run_os)/audio/libhy_hal_os_audio.la    \
                   $(lib_com)

libhy_hal_os_la_SOURCES    = $(_sources_com)
libhy_hal_os_la_CPPFLAGS   = $(_cppflags_com)
libhy_hal_os_la_CFLAGS     = $(_cflags_com)
libhy_hal_os_la_CXXFLAGS   = $(_cxxflags_com)
libhy_hal_os_la_LDFLAGS    = $(_ldflags_com)
libhy_hal_os_la_LIBADD     = $(_ldadd_com)

