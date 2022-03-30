# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    21/01 2022 19:30
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        21/01 2022      create the file
# 
#     last modified: 21/01 2022 19:30
# ===============================================================

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
bin_PROGRAMS    =

# ---------------------
# add necessary content
# ---------------------
_no_error_com   +=

_define_com     +=

_include_com    += \
                   -I$(top_srcdir)/include/$(run_os)            \
                   -I$(top_srcdir)/include/$(run_os)/thread     \
                   -I$(top_srcdir)/include/hy_hal               \
                   -I$(top_srcdir)/include

_sources_com    +=

_cppflags_com   += \
                   $(_no_error_com) \
                   $(_include_com)  \
                   $(_define_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += \
                   $(top_builddir)/src/lib@project_name@.la \
                   $(ldadd_com)

bin_PROGRAMS                += hy_thread_test
hy_thread_test_SOURCES      = $(_sources_com) hy_thread_test.c
hy_thread_test_CPPFLAGS     = $(_cppflags_com)
hy_thread_test_CFLAGS       = $(_cflags_com)
hy_thread_test_CXXFLAGS     = $(_cxxflags_com)
hy_thread_test_LDFLAGS      = $(_ldflags_com)
hy_thread_test_LDADD        = $(_ldadd_com)

