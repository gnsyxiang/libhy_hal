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

if COMPILE_SELECT_MCU_AT
_mcu_vender = at
endif

if COMPILE_SELECT_MCU_HC
_mcu_vender = hc
endif

SUBDIRS = $(_mcu_vender)

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
_elf_file       =
bin_PROGRAMS    =

# ---------------------
# add necessary content
# ---------------------
_no_error_com   +=

_include_com    += \
                   -I$(top_srcdir)/include/mcu  \
                   -I$(top_srcdir)/include

_sources_com    +=

_cppflags_com   += $(_no_error_com) $(_include_com)

_cflags_com     +=

_cxxflags_com   +=

_ldflags_com    +=

_ldadd_com      += $(top_builddir)/src/lib@project_name@.la

if COMPILE_LIBHY_MCU
_include_com    += $(HY_MCU_INCS)
_ldadd_com      += $(HY_MCU_LIBS)
endif

if COMPILE_SELECT_MCU_AT
if COMPILE_SELECT_MCU_AT32F4XX
_ldflags_com    += -T$(top_srcdir)/src/mcu/at/at32f4xx/AT32F407xG_FLASH.ld
endif
endif

_elf_file                   += hy_gpio_test
bin_PROGRAMS                += hy_gpio_test
hy_gpio_test_SOURCES        = $(_sources_com) hy_gpio_test.c
hy_gpio_test_CPPFLAGS       = $(_cppflags_com)
hy_gpio_test_CFLAGS         = $(_cflags_com)
hy_gpio_test_CXXFLAGS       = $(_cxxflags_com)
hy_gpio_test_LDFLAGS        = $(_ldflags_com) -Wl,--gc-sections,-Map=hy_gpio_test.map,-cref,-u,Reset_Handler
hy_gpio_test_LDADD          = $(_ldadd_com)

_elf_file                   += hy_template_test
bin_PROGRAMS                += hy_template_test
hy_template_test_SOURCES    = $(_sources_com) hy_template_test.c
hy_template_test_CPPFLAGS   = $(_cppflags_com)
hy_template_test_CFLAGS     = $(_cflags_com)
hy_template_test_CXXFLAGS   = $(_cxxflags_com)
hy_template_test_LDFLAGS    = $(_ldflags_com) -Wl,--gc-sections,-Map=hy_template_test.map,-cref,-u,Reset_Handler
hy_template_test_LDADD      = $(_ldadd_com)

install-exec-hook:
	for file in $(_elf_file); do \
		@gcc_prefix@objcopy -O binary $$file $$file.bin; \
	done
	$(INSTALL_PROGRAM_ENV) $(LIBTOOL) $(AM_LIBTOOLFLAGS) $(LIBTOOLFLAGS) --mode=install $(INSTALL_PROGRAM) *.bin "$(DESTDIR)$(bindir)$$dir"
