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
PKG_INSTALLDIR

# check m4 macro
CHECK_PROJECT_INFO
CHECK_VERSION_INFO
CHECK_DOXYGEN
# CHECK_JAVA

SELECT_VENDER
SELECT_CHIP
# SELECT_PRODUCT
# SELECT_LANGUAGE

AC_ARG_ENABLE([debug_info],
              [AS_HELP_STRING([--enable-debug_info], [enable support for output debug info])],
              [], [enable_debug_info=no])
AM_CONDITIONAL(COMPILE_DEBUG_INFO, [test "$enable_debug_info" = "yes"])

# Checks for programs.
AC_PROG_CXX
AC_PROG_AWK
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
# AC_PROG_RANLIB

CHECK_LIBHY_MCU
CHECK_LIBTINYALSA
CHECK_PTHREAD


# Checks for libraries.
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([dl], [dlopen])

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h inttypes.h stddef.h stdint.h stdlib.h string.h sys/socket.h sys/time.h sys/vfs.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT8_T
AC_TYPE_MODE_T
AC_TYPE_UINT8_T

# Checks for library functions.
# AC_FUNC_MALLOC
AC_FUNC_MKTIME
AC_FUNC_REALLOC
AC_CHECK_FUNCS([atexit gettimeofday inet_ntoa localtime_r memset mkfifo select socket strerror strstr])

AC_CONFIG_FILES([Makefile
                 $project_name.pc
                 build-script/Makefile
                 build-script/arterytek/Makefile
                 build-script/arterytek/at32f4xx/Makefile
                 build-script/eeasytech/Makefile
                 build-script/eeasytech/SV823/Makefile
                 build-script/fullhan/MC6810E/Makefile
                 build-script/fullhan/Makefile
                 build-script/pc/Makefile
                 build-script/pc/pc-chip/Makefile
                 doc/Makefile
                 example/Makefile
                 example/linux/Makefile
                 example/linux/audio/Makefile
                 example/linux/ipc/Makefile
                 example/linux/ipc/ipc_socket/Makefile
                 example/linux/log/Makefile
                 example/linux/thread/Makefile
                 example/mcu/Makefile
                 example/mcu/arterytek/Makefile
                 example/mcu/hdhc/Makefile
                 include/Makefile
                 include/hy_hal/Makefile
                 include/linux/Makefile
                 include/linux/audio/Makefile
                 include/linux/ipc/Makefile
                 include/linux/ipc/ipc_socket/Makefile
                 include/linux/log/Makefile
                 include/linux/thread/Makefile
                 include/mcu/Makefile
                 include/mcu/arterytek/Makefile
                 include/mcu/hdhc/Makefile
                 res/Makefile
                 src/Makefile
                 src/linux/Makefile
                 src/linux/audio/Makefile
                 src/linux/ipc/Makefile
                 src/linux/ipc/ipc_socket/Makefile
                 src/linux/log/Makefile
                 src/linux/thread/Makefile
                 src/mcu/Makefile
                 src/mcu/arterytek/Makefile
                 src/mcu/arterytek/at32f4xx/Makefile
                 src/mcu/hdhc/Makefile
                 ]) # for make dist
AC_OUTPUT

AC_MSG_RESULT([
               $PACKAGE $VERSION

               CC:                  ${CC}
               CPPFLAGS:            ${CPPFLAGS}
               CFLAGS:              ${CFLAGS}
               CXX:                 ${CXX}
               CXXFLAGS:            ${CXXFLAGS}
               LDFLAGS:             ${LDFLAGS}
               LIBS:                ${LIBS}

               prefix:              ${prefix}
               sysconfdir:          ${sysconfdir}
               libdir:              ${libdir}
               includedir:          ${includedir}
               pkgconfigdir:        ${pkgconfigdir}
               ])
