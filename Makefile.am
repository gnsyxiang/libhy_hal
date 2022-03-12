# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    05/03 2021 10:32
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        05/03 2021      create the file
# 
#     last modified: 05/03 2021 10:32
# ===============================================================

include res/doxygen.am

SUBDIRS = \
          build-script  \
          include       \
          src           \
          doc           \
          example       \
          res

# ------------------
# defining variables
# ------------------
_install_files_com  =
_dist_files_com     =

ACLOCAL_AMFLAGS     = -I m4

# ---------------------
# add necessary content
# ---------------------


# ------------------
# make install files
# ------------------
_install_files_com  += \
                       LICENSE      \
                       README.md
dist_doc_DATA       = $(_install_files_com)
pkgconfig_DATA      = @project_name@.pc

# ---------------
# make dist files
# ---------------
_dist_files_com     += \
                       autogen.sh   \
                       build.sh     \
                       @project_name@.pc.in
EXTRA_DIST          = $(_dist_files_com)

DISTCLEANFILES = @project_name@.pc

if COMPILE_LIBHY_MCU
install-exec-hook:
	hy_mcu=`sed -n "/hy_mcu/p" $(top_builddir)/@project_name@.pc`; \
	if [ $${#hy_mcu} -gt 0 ]; then \
		echo "Already added hy_mcu lib"; \
	else \
		sed -i "s/^Requires:/Requires: hy_mcu/g" $(top_builddir)/@project_name@.pc; \
	fi
endif
