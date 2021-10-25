dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_java.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    13/04 2021 09:01
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        13/04 2021      create the file
dnl 
dnl     last modified: 13/04 2021 09:01
dnl ===============================================================

# CHECK_JAVA()
# --------------------------------------------------------------
# check java

AC_DEFUN([CHECK_JAVA],
    [
        AC_CHECK_PROG([JAVA], [java], [yes])
        AM_CONDITIONAL([HAVA_JAVA_PROGRAM], [test "x$JAVA" = "xyes"])
    ])
