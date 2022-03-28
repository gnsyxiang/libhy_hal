# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    22/03 2022 15:41
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        22/03 2022      create the file
# 
#     last modified: 22/03 2022 15:41
# ===============================================================

# ------------------
# defining variables
# ------------------
_dist_file_com  =

# ------------------
# make install files
# ------------------


# ---------------
# make dist files
# ---------------
_dist_file_com  += config.sh

EXTRA_DIST      = $(_dist_file_com)

dist-hook:
	@echo "modify configure.ac"; \
		\
		sed -i 's/product=.*/product=$(product)/g' $(top_distdir)/build-script/$(vender)/$(chip)/config.sh; \
		\
		sed -i '/ build-script\//d' $(top_distdir)/configure.ac; \
		sed -i '/project_name.pc/a build-script/$(vender)/$(chip)/Makefile' $(top_distdir)/configure.ac; \
		sed -i '/project_name.pc/a build-script/$(vender)/Makefile' $(top_distdir)/configure.ac; \
		sed -i '/project_name.pc/a build-script/Makefile' $(top_distdir)/configure.ac; \
		\
		sed -i '/ example\//d' $(top_distdir)/configure.ac; \
		sed -i '/ doc\/Makefile/a example/Makefile' $(top_distdir)/configure.ac; \
		sed -i '/ doc\/Makefile/a example/$(run_os)/Makefile' $(top_distdir)/configure.ac; \
		sed -i '/ doc\/Makefile/a example/$(run_os)/$(vender)/Makefile' $(top_distdir)/configure.ac; \
		\
		sed -i '/ include\//d' $(top_distdir)/configure.ac; \
		sed -i '/ res\/Makefile/i include/Makefile' $(top_distdir)/configure.ac; \
		sed -i '/ res\/Makefile/i include/$(run_os)/Makefile' $(top_distdir)/configure.ac; \
		sed -i '/ res\/Makefile/i include/$(run_os)/$(vender)/Makefile' $(top_distdir)/configure.ac; \
		\
		sed -i '/ src\//d' $(top_distdir)/configure.ac; \
		sed -i '/ res\/Makefile/a src/Makefile' $(top_distdir)/configure.ac; \
		sed -i '/ res\/Makefile/a src/$(run_os)/Makefile' $(top_distdir)/configure.ac; \
		sed -i '/ res\/Makefile/a src/$(run_os)/$(vender)/Makefile' $(top_distdir)/configure.ac; \
		sed -i '/ res\/Makefile/a src/$(run_os)/$(vender)/$(chip)/Makefile' $(top_distdir)/configure.ac

