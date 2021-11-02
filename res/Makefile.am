# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    08/03 2021 08:16
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        08/03 2021      create the file
# 
#     last modified: 08/03 2021 08:16
# ===============================================================

# ------------------
# defining variables
# ------------------
_dist_file_com  =
resdir          = $(prefix)/res/@project_name@
res_DATA        =

# ------------------
# make install files
# ------------------
if COMPILE_LIBLOG4CPLUS
res_DATA        += log4cplus.rc
endif


# ---------------
# make dist files
# ---------------
_dist_file_com  += \
                   Doxyfile             \
                   get_project_info.sh

if COMPILE_LIBLOG4CPLUS
_dist_file_com  += log4cplus.rc
endif

EXTRA_DIST      = $(_dist_file_com)

