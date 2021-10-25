dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    check_doxygen.m4
dnl @brief   check doxygen
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    19/01 2021 11:29
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        19/01 2021      create the file
dnl 
dnl     last modified: 19/01 2021 11:29
dnl ===============================================================

# CHECK_DOXYGEN()
# --------------------------------------------------------------
# check doxygen

AC_DEFUN([CHECK_DOXYGEN],
    [
        DX_HTML_FEATURE(ON)
        DX_MAN_FEATURE(OFF)
        DX_RTF_FEATURE(OFF)
        DX_XML_FEATURE(OFF)
        DX_PDF_FEATURE(OFF)
        DX_PS_FEATURE(OFF)
        DX_CHM_FEATURE(OFF)
        DX_CHI_FEATURE(OFF)
        DX_INIT_DOXYGEN([PROJECT_NAME], [${top_srcdir}/res/Doxyfile], [doxygen])

        if test x"${enable_doxygen_doc}" == x"yes" ; then
            AC_CHECK_PROG([DOXYGEN], [doxygen], [yes])
            if test x"${DOXYGEN}" != x"yes" ; then
                AC_MSG_ERROR([Please install doxygen with 'sudo apt install -y doxygen'.])
            fi

            AC_CHECK_PROG([DOT], [dot], [yes])
            if test x"${DOT}" != x"yes" ; then
                AC_MSG_ERROR([Please install graphviz with 'sudo apt install -y graphviz'.])
            fi
        fi

        AM_CONDITIONAL([ENABLE_DOXYGEN], [test "$DX_FLAG_doc" = "1"])
        AM_CONDITIONAL([ENABLE_DOXYGEN_MAN], [test "$DX_FLAG_man" = "1"])
    ])
