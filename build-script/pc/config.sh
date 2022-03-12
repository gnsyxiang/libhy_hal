#!/usr/bin/env bash

chip=pc-chip

gcc_version=x86_64-linux-gnu
host=
cross_gcc_path=

configure_param=--with-target_os=linux

cppflag=-fstack-protector-all -Wno-error=unused-result -Wno-error=format-truncation= -Wno-error=stringop-truncation -Wno-error=stringop-overflow=
cflag=
cxxflag=
ldflag=-rdynamic
lib=

install_path=/opt/data/nfs/install/pc

