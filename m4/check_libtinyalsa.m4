dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_libtinyalsa.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    31/03 2022 11:07
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        31/03 2022      create the file
dnl 
dnl     last modified: 31/03 2022 11:07
dnl ===============================================================

# CHECK_LIBTINYALSA()
# --------------------------------------------------------------
# check libtinyalsa

AC_DEFUN([CHECK_LIBTINYALSA], [

    AC_ARG_ENABLE([tinyalsa],
        [AS_HELP_STRING([--enable-tinyalsa], [enable support for tinyalsa])],
            [], [enable_tinyalsa=no])

    case "$enable_tinyalsa" in
        yes)
            have_tinyalsa=no

            case "$PKG_CONFIG" in
                '') ;;
                *)
                    TINYALSA_LIBS=`$PKG_CONFIG --libs tinyalsa 2>/dev/null`

                    case "$TINYALSA_LIBS" in
                        '') ;;
                        *)
                            TINYALSA_LIBS="$TINYALSA_LIBS"
                            have_tinyalsa=yes
                        ;;
                    esac

                    TINYALSA_INCS=`$PKG_CONFIG --cflags tinyalsa 2>/dev/null`
                ;;
            esac

            case "$have_tinyalsa" in
                yes) ;;
                *)
                    save_LIBS="$LIBS"
                    LIBS=""
                    TINYALSA_LIBS=""

                    # clear cache
                    unset ac_cv_search_pcm_open
                    AC_SEARCH_LIBS([pcm_open], [tinyalsa],
                            [have_tinyalsa=yes
                                TINYALSA_LIBS="$LIBS"],
                            [have_tinyalsa=no],
                            [-ldl])
                    LIBS="$save_LIBS"
                ;;
            esac

            CPPFLAGS_SAVE=$CPPFLAGS
            CPPFLAGS="$CPPFLAGS $TINYALSA_INCS"
            AC_CHECK_HEADERS([tinyalsa/pcm.h], [], [have_tinyalsa=no])

            CPPFLAGS=$CPPFLAGS_SAVE
            AC_SUBST(TINYALSA_INCS)
            AC_SUBST(TINYALSA_LIBS)

            case "$have_tinyalsa" in
                yes)
                    AC_DEFINE(HAVE_TINYALSA, 1, [Define if the system has tinyalsa])
                ;;
                *)
                    AC_MSG_ERROR([tinyalsa is a must but can not be found. You should add the \
directory containing `tinyalsa.pc' to the `PKG_CONFIG_PATH' environment variable, \
or set `CPPFLAGS' and `LDFLAGS' directly for tinyalsa, or use `--disable-tinyalsa' \
to disable support for tinyalsa encryption])
                ;;
            esac
        ;;
    esac

    # check if we have and should use tinyalsa
    AM_CONDITIONAL(COMPILE_LIBTINYALSA, [test "$enable_tinyalsa" != "no" && test "$have_tinyalsa" = "yes"])
])

