#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])

m4_include([m4/check_project_info.m4])
m4_include([m4/get_cross_gcc_info.m4])

AC_INIT([PROJECT_NAME], [VERSION_NUM], [EMAIL_INFO], [PROJECT_NAME], [PROJECT_URL])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AM_SILENT_RULES([yes])
AM_PROG_AR

LT_INIT

PKG_PROG_PKG_CONFIG([0.24])

# check m4 macro
CHECK_PROJECT_INFO
CHECK_VERSION_INFO
CHECK_DOXYGEN
# CHECK_JAVA

# Checks for programs.
GCC_PREFIX_INFO
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
# AC_PROG_RANLIB
AC_PROG_MKDIR_P

SELECT_TARGET_OS
SELECT_MCU

CHECK_LIBHY_MCU

# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([dl], [dlopen])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h inttypes.h stdint.h stdlib.h string.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T
AC_TYPE_UINT32_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile
                 $project_name.pc
                 doc/Makefile
                 example/Makefile
                 example/linux/Makefile
                 example/mac/Makefile
                 example/mcu/Makefile
                 example/mcu/at/Makefile
                 example/mcu/hc/Makefile
                 example/rtos/Makefile
                 example/window/Makefile
                 include/Makefile
                 include/linux/Makefile
                 include/mac/Makefile
                 include/mcu/Makefile
                 include/mcu/at/Makefile
                 include/mcu/hc/Makefile
                 include/rtos/Makefile
                 include/window/Makefile
                 res/Makefile
                 src/Makefile
                 src/linux/Makefile
                 src/mac/Makefile
                 src/mcu/Makefile
                 src/mcu/at/Makefile
                 src/mcu/at/at32f4xx/Makefile
                 src/mcu/hc/Makefile
                 src/rtos/Makefile
                 src/window/Makefile])
AC_OUTPUT
