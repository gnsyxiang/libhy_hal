dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    select_product.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    11/03 2022 08:19
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        11/03 2022      create the file
dnl 
dnl     last modified: 11/03 2022 08:19
dnl ===============================================================

# SELECT_PRODUCT()
# --------------------------------------------------------------
# select product

AC_DEFUN([SELECT_PRODUCT],
    [
        product=""

        AC_ARG_WITH([product],
            [AS_HELP_STRING([
                --with-product=@<:@HY-pc|HY-8608|HY-8608E|HY-8608EV2|HY-MCU-6606@:>@],
                [select product about @<:@HY-pc|HY-8608|HY-8608E|HY-8608EV2|HY-MCU-6606@:>@ @<:@default=HY-pc@:>@])],
            [],
            [with_product=HY-pc])

        case "$with_product" in
            HY-pc)
                AC_DEFINE(HAVE_SELECT_PRODUCT_PC,  1, [select HY-pc product])
                product="HY-pc"
            ;;
            HY-8608)
                AC_DEFINE(HAVE_SELECT_PRODUCT_HY_8608,  1, [select HY-8608 product])
                product="HY-8608"
            ;;
            HY-8608E)
                AC_DEFINE(HAVE_SELECT_PRODUCT_HY_8608E,  1, [select HY-8608E product])
                product="HY-8608E"
            ;;
            HY-8608EV2)
                AC_DEFINE(HAVE_SELECT_PRODUCT_HY_8608EV2,  1, [select HY-8608EV2 product])
                product="HY-8608EV2"
            ;;
            HY-MCU-6606)
                AC_DEFINE(HAVE_SELECT_PRODUCT_HY_MCU_6606,  1, [select HY-MCU-6606 product])
                product="HY-MCU-6606"
            ;;
            *)
                AC_MSG_ERROR([bad value ${with_product} for --with-product=@<:@HY-pc|HY-8608|HY-8608E|HY-8608EV2|HY-MCU-6606@:>@])
            ;;
        esac

        AM_CONDITIONAL([COMPILE_SELECT_PRODUCT_PC],             [test "x$with_product" = "xpc"])
        AM_CONDITIONAL([COMPILE_SELECT_PRODUCT_HY_8608],        [test "x$with_product" = "xHY-8608"])
        AM_CONDITIONAL([COMPILE_SELECT_PRODUCT_HY_8608E],       [test "x$with_product" = "xHY-8608E"])
        AM_CONDITIONAL([COMPILE_SELECT_PRODUCT_HY_8608EV2],     [test "x$with_product" = "xHY-8608EV2"])
        AM_CONDITIONAL([COMPILE_SELECT_PRODUCT_HY_MCU_6606],     [test "x$with_product" = "xHY-MCU-6606"])
    ])

