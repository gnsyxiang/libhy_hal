# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    26/10 2021 08:39
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        26/10 2021      create the file
# 
#     last modified: 26/10 2021 08:39
# ===============================================================

# ------------------
# defining variables
# ------------------
_no_error_com   =
_define_com     =
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

_define_com     +=

_include_com    += \
                   -I$(top_srcdir)/include/linux    \
                   -I$(top_srcdir)/include

_sources_com    +=

_cppflags_com   += $(_no_error_com) $(_include_com) $(_define_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += $(top_builddir)/src/lib@project_name@.la

bin_PROGRAMS                += hy_dir_test
hy_dir_test_SOURCES         = $(_sources_com) hy_dir_test.c
hy_dir_test_CPPFLAGS        = $(_cppflags_com)
hy_dir_test_CFLAGS          = $(_cflags_com)
hy_dir_test_CXXFLAGS        = $(_cxxflags_com)
hy_dir_test_LDFLAGS         = $(_ldflags_com)
hy_dir_test_LDADD           = $(_ldadd_com)

bin_PROGRAMS                += hy_dlopen_test
hy_dlopen_test_SOURCES      = $(_sources_com) hy_dlopen_test.c
hy_dlopen_test_CPPFLAGS     = $(_cppflags_com)
hy_dlopen_test_CFLAGS       = $(_cflags_com)
hy_dlopen_test_CXXFLAGS     = $(_cxxflags_com)
hy_dlopen_test_LDFLAGS      = $(_ldflags_com)
hy_dlopen_test_LDADD        = $(_ldadd_com)

bin_PROGRAMS                += hy_log_test
hy_log_test_SOURCES         = $(_sources_com) hy_log_test.c
hy_log_test_CPPFLAGS        = $(_cppflags_com)
hy_log_test_CFLAGS          = $(_cflags_com)
hy_log_test_CXXFLAGS        = $(_cxxflags_com)
hy_log_test_LDFLAGS         = $(_ldflags_com)
hy_log_test_LDADD           = $(_ldadd_com)

bin_PROGRAMS                += hy_mem_test
hy_mem_test_SOURCES         = $(_sources_com) hy_mem_test.c
hy_mem_test_CPPFLAGS        = $(_cppflags_com)
hy_mem_test_CFLAGS          = $(_cflags_com)
hy_mem_test_CXXFLAGS        = $(_cxxflags_com)
hy_mem_test_LDFLAGS         = $(_ldflags_com)
hy_mem_test_LDADD           = $(_ldadd_com)

bin_PROGRAMS                += hy_template_test
hy_template_test_SOURCES    = $(_sources_com) hy_template_test.c
hy_template_test_CPPFLAGS   = $(_cppflags_com)
hy_template_test_CFLAGS     = $(_cflags_com)
hy_template_test_CXXFLAGS   = $(_cxxflags_com)
hy_template_test_LDFLAGS    = $(_ldflags_com)
hy_template_test_LDADD      = $(_ldadd_com)

bin_PROGRAMS                += hy_thread_test
hy_thread_test_SOURCES      = $(_sources_com) hy_thread_test.c
hy_thread_test_CPPFLAGS     = $(_cppflags_com)
hy_thread_test_CFLAGS       = $(_cflags_com)
hy_thread_test_CXXFLAGS     = $(_cxxflags_com)
hy_thread_test_LDFLAGS      = $(_ldflags_com)
hy_thread_test_LDADD        = $(_ldadd_com)

bin_PROGRAMS                += hy_time_test
hy_time_test_SOURCES        = $(_sources_com) hy_time_test.c
hy_time_test_CPPFLAGS       = $(_cppflags_com)
hy_time_test_CFLAGS         = $(_cflags_com)
hy_time_test_CXXFLAGS       = $(_cxxflags_com)
hy_time_test_LDFLAGS        = $(_ldflags_com)
hy_time_test_LDADD          = $(_ldadd_com)

bin_PROGRAMS                += hy_uart_test
hy_uart_test_SOURCES        = $(_sources_com) hy_uart_test.c
hy_uart_test_CPPFLAGS       = $(_cppflags_com)
hy_uart_test_CFLAGS         = $(_cflags_com)
hy_uart_test_CXXFLAGS       = $(_cxxflags_com)
hy_uart_test_LDFLAGS        = $(_ldflags_com)
hy_uart_test_LDADD          = $(_ldadd_com)

