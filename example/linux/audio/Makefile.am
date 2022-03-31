# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    31/03 2022 14:41
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        31/03 2022      create the file
# 
#     last modified: 31/03 2022 14:41
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
                   -I$(top_srcdir)/include/$(run_os)/audio      \
                   -I$(top_srcdir)/include/$(run_os)            \
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

if COMPILE_LIBTINYALSA
bin_PROGRAMS                    += hy_audio_recorder_demo
hy_audio_recorder_demo_SOURCES  = $(_sources_com) hy_audio_recorder_demo.c
hy_audio_recorder_demo_CPPFLAGS = $(_cppflags_com)
hy_audio_recorder_demo_CFLAGS   = $(_cflags_com)
hy_audio_recorder_demo_CXXFLAGS = $(_cxxflags_com)
hy_audio_recorder_demo_LDFLAGS  = $(_ldflags_com)
hy_audio_recorder_demo_LDADD    = $(_ldadd_com)
endif

