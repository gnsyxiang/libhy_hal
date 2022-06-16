dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_pthread.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    15/06 2022 17:31
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        15/06 2022      create the file
dnl 
dnl     last modified: 15/06 2022 17:31
dnl ===============================================================

# CHECK_PTHREAD()
# --------------------------------------------------------------
# check pthread

AC_DEFUN([CHECK_PTHREAD],
    [
        have_pthread_setname_np=no

        # clear cache
        unset ac_cv_search_pthread_setname_np
        AC_SEARCH_LIBS([pthread_setname_np],
            [pthread],
            [have_pthread_setname_np=yes],
            [have_pthread_setname_np=no],
            [])

        case "$have_pthread_setname_np" in
            yes)
                AC_DEFINE(HAVE_PTHREAD_SETNAME_NP, 1, [Define if the pthread has pthread_setname_np])
            ;;
            *)
                AC_MSG_WARN([don't have pthread_setname_np])
            ;;
        esac
    ])

