#!/usr/bin/env bash

chip=MC6810E

product=HY-6810

language=cn en

host=arm-mol-linux-uclibcgnueabihf
cross_gcc_path=/mnt/data/toolchain/fullhan/MC6810E/molchipv500-armgcc-uclibc/bin/arm-mol-linux-uclibcgnueabihf-

configure_param=

cppflag=-fstack-protector-all -std=gnu99
cflag=
cxxflag=
ldflag=-rdynamic
lib=

install_path=/mnt/data/nfs/fullhan/MC6810E

