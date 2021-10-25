dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_libhy_utils.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    25/10 2021 18:38
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        25/10 2021      create the file
dnl 
dnl     last modified: 25/10 2021 18:38
dnl ===============================================================

# CHECK_LIBHY_UTILS()
# --------------------------------------------------------------
# check libhy_utils

AC_DEFUN([CHECK_LIBHY_UTILS], [

    AC_ARG_ENABLE([hy_utils],
        [AS_HELP_STRING([--disable-hy_utils], [disable support for hy_utils])],
            [], [enable_hy_utils=yes])

    case "$enable_hy_utils" in
        yes)
            have_hy_utils=no

            case "$PKG_CONFIG" in
                '') ;;
                *)
                    HY_UTILS_LIBS=`$PKG_CONFIG --libs hy_utils 2>/dev/null`

                    case "$HY_UTILS_LIBS" in
                        '') ;;
                        *)
                            HY_UTILS_LIBS="$HY_UTILS_LIBS"
                            have_hy_utils=yes
                        ;;
                    esac

                    HY_UTILS_INCS=`$PKG_CONFIG --cflags hy_utils 2>/dev/null`
                ;;
            esac

            case "$have_hy_utils" in
                yes) ;;
                *)
                    save_LIBS="$LIBS"
                    LIBS=""
                    HY_UTILS_LIBS=""

                    # clear cache
                    unset ac_cv_search_HyLogCreate
                    AC_SEARCH_LIBS([HyLogCreate], [hy_utils],
                            [have_hy_utils=yes
                                HY_UTILS_LIBS="$LIBS"],
                            [have_hy_utils=no],
                            [])
                    LIBS="$save_LIBS"
                ;;
            esac

            CPPFLAGS_SAVE=$CPPFLAGS
            CPPFLAGS="$CPPFLAGS $HY_UTILS_INCS"
            AC_CHECK_HEADERS([hy_utils/hy_log.h], [], [have_hy_utils=no])

            CPPFLAGS=$CPPFLAGS_SAVE
            AC_SUBST(HY_UTILS_INCS)
            AC_SUBST(HY_UTILS_LIBS)

            case "$have_hy_utils" in
                yes)
                    AC_DEFINE(HAVE_HY_UTILS, 1, [Define if the system has hy_utils])
                ;;
                *)
                    AC_MSG_ERROR([hy_utils is a must but can not be found. You should add the \
directory containing `hy_utils.pc' to the `PKG_CONFIG_PATH' environment variable, \
or set `CPPFLAGS' and `LDFLAGS' directly for hy_utils, or use `--disable-hy_utils' \
to disable support for hy_utils encryption])
                ;;
            esac
        ;;
    esac

    # check if we have and should use hy_utils
    AM_CONDITIONAL(COMPILE_LIBHY_UTILS, [test "$enable_hy_utils" != "no" && test "$have_hy_utils" = "yes"])
])

