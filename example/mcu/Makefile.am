# ===============================================================
# 
# Release under GPLv-3.0.
# 
# @file    Makefile.am
# @brief   
# @author  gnsyxiang <gnsyxiang@163.com>
# @date    29/10 2021 20:55
# @version v0.0.1
# 
# @since    note
# @note     note
# 
#     change log:
#     NO.     Author              Date            Modified
#     00      zhenquan.qiu        29/10 2021      create the file
# 
#     last modified: 29/10 2021 20:55
# ===============================================================

SUBDIRS = $(vender)

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
_elf_file       =
bin_PROGRAMS    =

# ---------------------
# add necessary content
# ---------------------
_no_error_com   +=

_define_com     +=

_include_com    += \
                   -I$(top_srcdir)/include/$(run_os)    \
                   -I$(top_srcdir)/include/hy_hal       \
                   -I$(top_srcdir)/include

_sources_com    +=

_cppflags_com   += \
                   $(_no_error_com)     \
                   $(_include_com)      \
                   $(_define_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += \
                   $(top_builddir)/src/lib@project_name@.la \
                   $(ldadd_com)

if COMPILE_SELECT_VENDER_ARTERYTEK
if COMPILE_SELECT_CHIP_AT32F4XX
_ldflags_com    += -T$(top_srcdir)/src/$(run_os)/$(vender)/$(chip)/AT32F407xG_FLASH.ld
endif
endif

# _elf_file                   += hy_gpio_demo
# bin_PROGRAMS                += hy_gpio_demo
# hy_gpio_demo_SOURCES        = $(_sources_com) hy_gpio_demo.c
# hy_gpio_demo_CPPFLAGS       = $(_cppflags_com)
# hy_gpio_demo_CFLAGS         = $(_cflags_com)
# hy_gpio_demo_CXXFLAGS       = $(_cxxflags_com)
# hy_gpio_demo_LDFLAGS        = $(_ldflags_com) -Wl,--gc-sections,-Map=hy_gpio_demo.map,-cref,-u,Reset_Handler
# hy_gpio_demo_LDADD          = $(_ldadd_com)
# 
# _elf_file                   += hy_template_demo
# bin_PROGRAMS                += hy_template_demo
# hy_template_demo_SOURCES    = $(_sources_com) hy_template_demo.c
# hy_template_demo_CPPFLAGS   = $(_cppflags_com)
# hy_template_demo_CFLAGS     = $(_cflags_com)
# hy_template_demo_CXXFLAGS   = $(_cxxflags_com)
# hy_template_demo_LDFLAGS    = $(_ldflags_com) -Wl,--gc-sections,-Map=hy_template_demo.map,-cref,-u,Reset_Handler
# hy_template_demo_LDADD      = $(_ldadd_com)
# 
# install-exec-hook:
# 	for file in $(_elf_file); do \
# 		@gcc_prefix@objcopy -O binary $$file $$file.bin; \
# 	done
# 	$(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) *.bin "$(DESTDIR)$(bindir)$$dir"

