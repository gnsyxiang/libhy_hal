# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    05/03 2021 10:38
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        05/03 2021      create the file
# 
#     last modified: 05/03 2021 10:38
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
_no_error_com   =
_include_com    =
_sources_com    =
_cppflags_com   =
_cflags_com     =
_cxxflags_com   =
_ldflags_com    =
_ldadd_com      =
bin_PROGRAMS    =

# ---------------------
# add necessary content
# ---------------------
_no_error_com   +=

_include_com    += -I$(top_srcdir)/include

_sources_com    +=

_cppflags_com   += $(_no_error_com) $(_include_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += $(top_builddir)/src/lib@project_name@.la

bin_PROGRAMS    += main
main_SOURCES    = $(_sources_com) main.c
main_CPPFLAGS   = $(_cppflags_com)
main_CFLAGS     = $(_cflags_com)
main_CXXFLAGS   = $(_cxxflags_com)
main_LDFLAGS    = $(_ldflags_com)
main_LDADD      = $(_ldadd_com)
