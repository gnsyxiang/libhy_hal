dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_liblog4cplus.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    30/10 2021 14:41
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        30/10 2021      create the file
dnl 
dnl     last modified: 30/10 2021 14:41
dnl ===============================================================

# CHECK_LIBLOG4CPLUS()
# --------------------------------------------------------------
# check liblog4cplus

AC_DEFUN([CHECK_LIBLOG4CPLUS], [

    AC_LANG_PUSH([C++])

    AC_ARG_ENABLE([log4cplus],
        [AS_HELP_STRING([--enable-log4cplus], [enable support for log4cplus])],
            [], [enable_log4cplus=no])

    case "$enable_log4cplus" in
        yes)
            have_log4cplus=no

            case "$PKG_CONFIG" in
                '') ;;
                *)
                    LOG4CPLUS_LIBS=`$PKG_CONFIG --libs log4cplus 2>/dev/null`

                    case "$LOG4CPLUS_LIBS" in
                        '') ;;
                        *)
                            LOG4CPLUS_LIBS="$LOG4CPLUS_LIBS"
                            have_log4cplus=yes
                        ;;
                    esac

                    LOG4CPLUS_INCS=`$PKG_CONFIG --cflags log4cplus 2>/dev/null`
                ;;
            esac

            case "$have_log4cplus" in
                yes) ;;
                *)
                    save_LIBS="$LIBS"
                    LIBS=""
                    LOG4CPLUS_LIBS=""

                    # clear cache
                    unset ac_cv_search_SSL_new
                    AC_SEARCH_LIBS([_ZN9log4cplus6Logger7getRootEv], [log4cplus],
                            [have_log4cplus=yes
                                LOG4CPLUS_LIBS="$LIBS"],
                            [have_log4cplus=no],
                            [])
                    LIBS="$save_LIBS"
                ;;
            esac

            CPPFLAGS_SAVE=$CPPFLAGS
            CPPFLAGS="$CPPFLAGS $LOG4CPLUS_INCS"
            AC_CHECK_HEADERS([log4cplus/logger.h], [], [have_log4cplus=no])

            CPPFLAGS=$CPPFLAGS_SAVE
            AC_SUBST(LOG4CPLUS_INCS)
            AC_SUBST(LOG4CPLUS_LIBS)

            case "$have_log4cplus" in
                yes)
                    AC_DEFINE(HAVE_LOG4CPLUS, 1, [Define if the system has log4cplus])
                ;;
                *)
                    AC_MSG_ERROR([log4cplus is a must but can not be found. You should add the \
directory containing `log4cplus.pc' to the `PKG_CONFIG_PATH' environment variable, \
or set `CPPFLAGS' and `LDFLAGS' directly for log4cplus, or use `--disable-log4cplus' \
to disable support for log4cplus encryption])
                ;;
            esac
        ;;
    esac

    AC_LANG_POP([C++])

    # check if we have and should use log4cplus
    AM_CONDITIONAL(COMPILE_LIBLOG4CPLUS, [test "$enable_log4cplus" != "no" && test "$have_log4cplus" = "yes"])
])

