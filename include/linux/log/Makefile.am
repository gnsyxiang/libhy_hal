# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    21/04 2022 14:23
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        21/04 2022      create the file
# 
#     last modified: 21/04 2022 14:23
# ===============================================================

# ------------------
# defining variables
# ------------------
_no_install_headers_com =
_install_headers_com    =

install_includedir      = $(includedir)/@project_name@

# ---------------------
# add necessary content
# ---------------------
_no_install_headers_com += \
                           dynamic_array.h          \
                           epoll_helper.h           \
                           format_cb.h              \
                           log_fifo.h               \
                           log_file.h               \
                           log_private.h            \
                           process_handle_data.h    \
                           process_ipc_client.h     \
                           process_ipc_server.h     \
                           process_single.h         \
                           socket_ipc_client.h      \
                           socket_ipc_server.h      \
                           thread_specific_data.h

_install_headers_com    += hy_log.h

# ------------------
# make install files
# ------------------
noinst_HEADERS          = $(_no_install_headers_com)
install_include_HEADERS = $(_install_headers_com)

