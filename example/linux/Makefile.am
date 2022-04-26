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

SUBDIRS = \
          audio     \
          ipc       \
          log       \
          thread

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
                   -I$(top_srcdir)/include/$(run_os)/log        \
                   -I$(top_srcdir)/include/$(run_os)/thread     \
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

bin_PROGRAMS                    += hy_dir_demo
hy_dir_demo_SOURCES             = $(_sources_com) hy_dir_demo.c
hy_dir_demo_CPPFLAGS            = $(_cppflags_com)
hy_dir_demo_CFLAGS              = $(_cflags_com)
hy_dir_demo_CXXFLAGS            = $(_cxxflags_com)
hy_dir_demo_LDFLAGS             = $(_ldflags_com)
hy_dir_demo_LDADD               = $(_ldadd_com)

bin_PROGRAMS                    += hy_dlopen_demo
hy_dlopen_demo_SOURCES          = $(_sources_com) hy_dlopen_demo.c
hy_dlopen_demo_CPPFLAGS         = $(_cppflags_com)
hy_dlopen_demo_CFLAGS           = $(_cflags_com)
hy_dlopen_demo_CXXFLAGS         = $(_cxxflags_com)
hy_dlopen_demo_LDFLAGS          = $(_ldflags_com)
hy_dlopen_demo_LDADD            = $(_ldadd_com)

bin_PROGRAMS                    += hy_gpio_demo
hy_gpio_demo_SOURCES            = $(_sources_com) hy_gpio_demo.c
hy_gpio_demo_CPPFLAGS           = $(_cppflags_com)
hy_gpio_demo_CFLAGS             = $(_cflags_com)
hy_gpio_demo_CXXFLAGS           = $(_cxxflags_com)
hy_gpio_demo_LDFLAGS            = $(_ldflags_com)
hy_gpio_demo_LDADD              = $(_ldadd_com)

bin_PROGRAMS                    += hy_hal_fifo_read_demo
hy_hal_fifo_read_demo_SOURCES   = $(_sources_com) hy_hal_fifo_read_demo.c
hy_hal_fifo_read_demo_CPPFLAGS  = $(_cppflags_com)
hy_hal_fifo_read_demo_CFLAGS    = $(_cflags_com)
hy_hal_fifo_read_demo_CXXFLAGS  = $(_cxxflags_com)
hy_hal_fifo_read_demo_LDFLAGS   = $(_ldflags_com)
hy_hal_fifo_read_demo_LDADD     = $(_ldadd_com)

bin_PROGRAMS                    += hy_hal_fifo_write_demo
hy_hal_fifo_write_demo_SOURCES  = $(_sources_com) hy_hal_fifo_write_demo.c
hy_hal_fifo_write_demo_CPPFLAGS = $(_cppflags_com)
hy_hal_fifo_write_demo_CFLAGS   = $(_cflags_com)
hy_hal_fifo_write_demo_CXXFLAGS = $(_cxxflags_com)
hy_hal_fifo_write_demo_LDFLAGS  = $(_ldflags_com)
hy_hal_fifo_write_demo_LDADD    = $(_ldadd_com)

bin_PROGRAMS                    += hy_hotplug_demo
hy_hotplug_demo_SOURCES         = $(_sources_com) hy_hotplug_demo.c
hy_hotplug_demo_CPPFLAGS        = $(_cppflags_com)
hy_hotplug_demo_CFLAGS          = $(_cflags_com)
hy_hotplug_demo_CXXFLAGS        = $(_cxxflags_com)
hy_hotplug_demo_LDFLAGS         = $(_ldflags_com)
hy_hotplug_demo_LDADD           = $(_ldadd_com)

bin_PROGRAMS                    += hy_list_demo
hy_list_demo_SOURCES            = $(_sources_com) hy_list_demo.c
hy_list_demo_CPPFLAGS           = $(_cppflags_com)
hy_list_demo_CFLAGS             = $(_cflags_com)
hy_list_demo_CXXFLAGS           = $(_cxxflags_com)
hy_list_demo_LDFLAGS            = $(_ldflags_com)
hy_list_demo_LDADD              = $(_ldadd_com)

bin_PROGRAMS                    += hy_mem_demo
hy_mem_demo_SOURCES             = $(_sources_com) hy_mem_demo.c
hy_mem_demo_CPPFLAGS            = $(_cppflags_com)
hy_mem_demo_CFLAGS              = $(_cflags_com)
hy_mem_demo_CXXFLAGS            = $(_cxxflags_com)
hy_mem_demo_LDFLAGS             = $(_ldflags_com)
hy_mem_demo_LDADD               = $(_ldadd_com)

bin_PROGRAMS                    += hy_pipe_demo
hy_pipe_demo_SOURCES            = $(_sources_com) hy_pipe_demo.c
hy_pipe_demo_CPPFLAGS           = $(_cppflags_com)
hy_pipe_demo_CFLAGS             = $(_cflags_com)
hy_pipe_demo_CXXFLAGS           = $(_cxxflags_com)
hy_pipe_demo_LDFLAGS            = $(_ldflags_com)
hy_pipe_demo_LDADD              = $(_ldadd_com)

bin_PROGRAMS                    += hy_sd_demo
hy_sd_demo_SOURCES              = $(_sources_com) hy_sd_demo.c
hy_sd_demo_CPPFLAGS             = $(_cppflags_com)
hy_sd_demo_CFLAGS               = $(_cflags_com)
hy_sd_demo_CXXFLAGS             = $(_cxxflags_com)
hy_sd_demo_LDFLAGS              = $(_ldflags_com)
hy_sd_demo_LDADD                = $(_ldadd_com)

bin_PROGRAMS                    += hy_socket_client_demo
hy_socket_client_demo_SOURCES   = $(_sources_com) hy_socket_client_demo.c
hy_socket_client_demo_CPPFLAGS  = $(_cppflags_com)
hy_socket_client_demo_CFLAGS    = $(_cflags_com)
hy_socket_client_demo_CXXFLAGS  = $(_cxxflags_com)
hy_socket_client_demo_LDFLAGS   = $(_ldflags_com)
hy_socket_client_demo_LDADD     = $(_ldadd_com)

bin_PROGRAMS                    += hy_socket_server_demo
hy_socket_server_demo_SOURCES   = $(_sources_com) hy_socket_server_demo.c
hy_socket_server_demo_CPPFLAGS  = $(_cppflags_com)
hy_socket_server_demo_CFLAGS    = $(_cflags_com)
hy_socket_server_demo_CXXFLAGS  = $(_cxxflags_com)
hy_socket_server_demo_LDFLAGS   = $(_ldflags_com)
hy_socket_server_demo_LDADD     = $(_ldadd_com)

bin_PROGRAMS                    += hy_storage_demo
hy_storage_demo_SOURCES         = $(_sources_com) hy_storage_demo.c
hy_storage_demo_CPPFLAGS        = $(_cppflags_com)
hy_storage_demo_CFLAGS          = $(_cflags_com)
hy_storage_demo_CXXFLAGS        = $(_cxxflags_com)
hy_storage_demo_LDFLAGS         = $(_ldflags_com)
hy_storage_demo_LDADD           = $(_ldadd_com)

bin_PROGRAMS                    += hy_template_demo
hy_template_demo_SOURCES        = $(_sources_com) hy_template_demo.c
hy_template_demo_CPPFLAGS       = $(_cppflags_com)
hy_template_demo_CFLAGS         = $(_cflags_com)
hy_template_demo_CXXFLAGS       = $(_cxxflags_com)
hy_template_demo_LDFLAGS        = $(_ldflags_com)
hy_template_demo_LDADD          = $(_ldadd_com)

bin_PROGRAMS                    += hy_time_demo
hy_time_demo_SOURCES            = $(_sources_com) hy_time_demo.c
hy_time_demo_CPPFLAGS           = $(_cppflags_com)
hy_time_demo_CFLAGS             = $(_cflags_com)
hy_time_demo_CXXFLAGS           = $(_cxxflags_com)
hy_time_demo_LDFLAGS            = $(_ldflags_com)
hy_time_demo_LDADD              = $(_ldadd_com)

bin_PROGRAMS                    += hy_uart_demo
hy_uart_demo_SOURCES            = $(_sources_com) hy_uart_demo.c
hy_uart_demo_CPPFLAGS           = $(_cppflags_com)
hy_uart_demo_CFLAGS             = $(_cflags_com)
hy_uart_demo_CXXFLAGS           = $(_cxxflags_com)
hy_uart_demo_LDFLAGS            = $(_ldflags_com)
hy_uart_demo_LDADD              = $(_ldadd_com)

