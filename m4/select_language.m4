dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    select_language.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    11/03 2022 15:19
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        11/03 2022      create the file
dnl 
dnl     last modified: 11/03 2022 15:19
dnl ===============================================================

# SELECT_LANGUAGE()
# --------------------------------------------------------------
# select language

AC_DEFUN([SELECT_LANGUAGE],
    [
        language=""

        AC_ARG_WITH([language],
            [AS_HELP_STRING([--with-language=@<:@cn|en@:>@], [select language about @<:@cn|en@:>@ @<:@default=cn@:>@])],
            [],
            [with_language=cn])

        case "$with_language" in
            cn)
                AC_DEFINE(HAVE_SELECT_LANGUAGE_CN,  1, [select cn language])
                language="cn"
            ;;
            en)
                AC_DEFINE(HAVE_SELECT_LANGUAGE_EN,  1, [select en language])
                language="en"
            ;;
            *)
                AC_MSG_ERROR([bad value ${with_language} for --with-language=@<:@cn|en@:>@])
            ;;
        esac

        AC_SUBST(language)

        AM_CONDITIONAL([COMPILE_SELECT_LANGUAGE_CN],        [test "x$with_language" = "xcn"])
        AM_CONDITIONAL([COMPILE_SELECT_LANGUAGE_EN],        [test "x$with_language" = "xen"])
    ])

