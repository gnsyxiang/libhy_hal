dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    select_target_os.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    25/10 2021 18:43
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        25/10 2021      create the file
dnl 
dnl     last modified: 25/10 2021 18:43
dnl ===============================================================

# SELECT_TARGET_OS()
# --------------------------------------------------------------
# select target system os

AC_DEFUN([SELECT_TARGET_OS],
    [
        AC_ARG_WITH([target_os],
            [AS_HELP_STRING([--with-target_os=@<:@linux|mac|window|rtos|mcu@:>@], [select system os about @<:@linux|mac|window|rtos|mcu@:>@ @<:@default=linux@:>@])],
            [],
            [with_target_os=linux])

        case "$with_target_os" in
            linux)  AC_DEFINE(HAVE_SELECT_TARGET_OS_LINUX,  1, [select linux target system]) ;;
            mac)    AC_DEFINE(HAVE_SELECT_TARGET_OS_MAC,    1, [select mac target system]) ;;
            window) AC_DEFINE(HAVE_SELECT_TARGET_OS_WINDOW, 1, [select window target system]) ;;
            rtos)   AC_DEFINE(HAVE_SELECT_TARGET_OS_RTOS,   1, [select rtos target system]) ;;
            mcu)    AC_DEFINE(HAVE_SELECT_TARGET_OS_MCU,    1, [select mcu target system]) ;;
            *)      AC_MSG_ERROR([bad value ${with_target_os} for --with-target_os=@<:@linux|mac|window|rtos|mcu@:>@]) ;;
        esac

        AM_CONDITIONAL([COMPILE_SELECT_TARGET_OS_LINUX],    [test "x$with_target_os" = "xlinux"])
        AM_CONDITIONAL([COMPILE_SELECT_TARGET_OS_MAC],      [test "x$with_target_os" = "xmac"])
        AM_CONDITIONAL([COMPILE_SELECT_TARGET_OS_WINDOW],   [test "x$with_target_os" = "xwindow"])
        AM_CONDITIONAL([COMPILE_SELECT_TARGET_OS_RTOS],     [test "x$with_target_os" = "xrtos"])
        AM_CONDITIONAL([COMPILE_SELECT_TARGET_OS_MCU],      [test "x$with_target_os" = "xmcu"])
    ])

