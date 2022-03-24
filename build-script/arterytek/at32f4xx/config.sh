#!/usr/bin/env bash

chip=at32f4xx

product=HY-MCU-6606

language=cn en

host=arm-none-eabi
cross_gcc_path=/opt/toolchains/MCU/gcc-arm-none-eabi-5_4-2016q3/bin/arm-none-eabi-

configure_param=--with-target_os=mcu --with-mcu=at32f4xx

cppflag=-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DAT32F407VGT7 -DAT_START_F407_V1_0 -DUSE_STDPERIPH_DRIVER -DSYSCLK_FREQ_240MHz=240000000 -I/opt/data/nfs/install/MCU/at/at32f4xx/include/hy_mcu
cflag=
cxxflag=
ldflag=-specs=nano.specs -specs=nosys.specs -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
lib=

install_path=/opt/data/nfs/install/MCU/arterytek/at32f4xx

