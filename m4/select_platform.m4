dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    select_platform.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    10/03 2022 14:49
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        10/03 2022      create the file
dnl 
dnl     last modified: 10/03 2022 14:49
dnl ===============================================================

# SELECT_PLATFORM()
# --------------------------------------------------------------
# select platform

AC_DEFUN([SELECT_PLATFORM],
    [
        platform=""

        AC_ARG_WITH([platform],
            [AS_HELP_STRING([--with-platform=@<:@linux|mcu@:>@], [select platform about @<:@linux|mcu@:>@ @<:@default=linux@:>@])],
            [],
            [with_platform=linux])

        case "$with_platform" in
            linux)
                AC_DEFINE(HAVE_SELECT_PLATFORM_LINUX,  1, [select linux platform])
                platform="linux"
            ;;
            mcu)
                AC_DEFINE(HAVE_SELECT_PLATFORM_MCU,  1, [select mcu platform])
                platform="mcu"
            ;;
            *)
                AC_MSG_ERROR([bad value ${with_platform} for --with-platform=@<:@linux|mcu@:>@])
            ;;
        esac

        AM_CONDITIONAL([COMPILE_SELECT_PLATFORM_LINUX],     [test "x$with_platform" = "xlinux"])
        AM_CONDITIONAL([COMPILE_SELECT_PLATFORM_MCU],       [test "x$with_platform" = "xmcu"])
    ])

