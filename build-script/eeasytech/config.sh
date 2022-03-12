#!/usr/bin/env bash

chip=SV823

host=arm-linux-gnueabihf
cross_gcc_path=/opt/toolchains/eeasytech/SV823/gcc-linaro-7.5.0-2019.12-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-

configure_param=--with-target_os=linux

cppflag=-fstack-protector-all -Wno-error=unused-result -Wno-error=format-truncation= -Wno-error=stringop-overflow=
cflag=
cxxflag=
ldflag=-rdynamic
lib=

install_path=/opt/data/nfs/install/eeasytech/SV823

