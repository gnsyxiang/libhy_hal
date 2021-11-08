dnl ===============================================================
dnl 
dnl Release under GPLv-3.0.
dnl 
dnl @file    select_mcu.m4
dnl @brief   
dnl @author  gnsyxiang <gnsyxiang@163.com>
dnl @date    05/11 2021 19:36
dnl @version v0.0.1
dnl 
dnl @since    note
dnl @note     note
dnl 
dnl     change log:
dnl     NO.     Author              Date            Modified
dnl     00      zhenquan.qiu        05/11 2021      create the file
dnl 
dnl     last modified: 05/11 2021 19:36
dnl ===============================================================

# SELECT_MCU()
# --------------------------------------------------------------
# select mcu

AC_DEFUN([SELECT_MCU],
    [
        mcu_vender=""

        AC_ARG_WITH([mcu],
            [AS_HELP_STRING([--with-mcu=@<:@at32f4xx|hc32l13x|hc32f003@:>@], [select system os about @<:@at32f4xx|hc32l13x|hc32f003@:>@ @<:@default=at32f4xx@:>@])],
            [],
            [with_mcu=at32f4xx])

        case "$with_mcu" in
            at32f4xx)
                AC_DEFINE(HAVE_SELECT_MCU_AT32F4XX,  1, [select mcu at32f4xx])
                mcu_vender="at"
                AT32F4XX_DRIVER="AT32F4xx_StdPeriph_Lib_V1.3.2"
                AC_SUBST(AT32F4XX_DRIVER)
            ;;
            hc32l13x)
                AC_DEFINE(HAVE_SELECT_MCU_HC32L13X,  1, [select mcu hc32l13x])
                mcu_vender="hc"
                HC32L13X_DRIVER=""
                AC_SUBST(HC32L13X_DRIVER)
            ;;
            hc32f003)
                AC_DEFINE(HAVE_SELECT_MCU_HC32F003,  1, [select mcu hc32f003])
                mcu_vender="hc"
                HC32F003_DRIVER=""
                AC_SUBST(HC32F003_DRIVER)
            ;;
            *) AC_MSG_ERROR([bad value ${with_mcu} for --with-mcu=@<:@at32f4xx|hc32l13x|hc32f003@:>@]) ;;
        esac

        AM_CONDITIONAL([COMPILE_SELECT_MCU_AT],         [test "x$mcu_vender" = "xat"])
        AM_CONDITIONAL([COMPILE_SELECT_MCU_AT32F4XX],   [test "x$with_mcu" = "xat32f4xx"])

        AM_CONDITIONAL([COMPILE_SELECT_MCU_HC],         [test "x$mcu_vender" = "xhc"])
        AM_CONDITIONAL([COMPILE_SELECT_MCU_HC32L13X],   [test "x$with_mcu" = "xhc32l13x"])
        AM_CONDITIONAL([COMPILE_SELECT_MCU_HC32F003],   [test "x$with_mcu" = "xhc32f003"])
    ])

