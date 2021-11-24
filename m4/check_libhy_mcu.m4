dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_libhy_mcu.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    08/05 2021 10:29
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        08/05 2021      create the file
dnl 
dnl     last modified: 08/05 2021 10:29
dnl ===============================================================

# CHECK_LIBHY_MCU()
# --------------------------------------------------------------
# check libhy_mcu

AC_DEFUN([CHECK_LIBHY_MCU], [

    AC_ARG_ENABLE([hy_mcu],
        [AS_HELP_STRING([--disable-hy_mcu], [disable support for hy_mcu])],
            [], [enable_hy_mcu=yes])

    case "$enable_hy_mcu" in
        yes)
            have_hy_mcu=no

            case "$PKG_CONFIG" in
                '') ;;
                *)
                    HY_MCU_LIBS=`$PKG_CONFIG --libs hy_mcu 2>/dev/null`

                    case "$HY_MCU_LIBS" in
                        '') ;;
                        *)
                            HY_MCU_LIBS="$HY_MCU_LIBS"
                            have_hy_mcu=yes
                        ;;
                    esac

                    HY_MCU_INCS=`$PKG_CONFIG --cflags hy_mcu 2>/dev/null`
                ;;
            esac

            case "$have_hy_mcu" in
                yes) ;;
                *)
                    save_LIBS="$LIBS"
                    LIBS=""
                    HY_MCU_LIBS=""

                    # clear cache
                    unset ac_cv_search_HyFuncCheck
                    AC_SEARCH_LIBS([HyFuncCheck], [hy_mcu],
                            [have_hy_mcu=yes
                                HY_MCU_LIBS="$LIBS"],
                            [have_hy_mcu=no],
                            [])
                    LIBS="$save_LIBS"
                ;;
            esac

            CPPFLAGS_SAVE=$CPPFLAGS
            CPPFLAGS="$CPPFLAGS $HY_MCU_INCS"
            AC_CHECK_HEADERS([hy_mcu/hy_func_check.h], [], [have_hy_mcu=no])

            CPPFLAGS=$CPPFLAGS_SAVE
            AC_SUBST(HY_MCU_INCS)
            AC_SUBST(HY_MCU_LIBS)

            case "$have_hy_mcu" in
                yes)
                    AC_DEFINE(HAVE_HY_MCU, 1, [Define if the system has hy_mcu])
                ;;
                *)
                    AC_MSG_WARN([hy_mcu is a must but can not be found. You should add the \
directory containing `hy_mcu.pc' to the `PKG_CONFIG_PATH' environment variable, \
or set `CPPFLAGS' and `LDFLAGS' directly for hy_mcu, or use `--disable-hy_mcu' \
to disable support for hy_mcu encryption])
                ;;
            esac
        ;;
    esac

    # check if we have and should use hy_mcu
    AM_CONDITIONAL(COMPILE_LIBHY_MCU, [test "$enable_hy_mcu" != "no" && test "$have_hy_mcu" = "yes"])
])

