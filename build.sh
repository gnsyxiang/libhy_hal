#!/usr/bin/env bash

# set -x

help_info()
{
    echo "eg: ./build.sh pc/arm/mcu [_build]"
    exit
}

if [ $# -gt 2 -o $# -lt 1 ]; then
    help_info
fi

data_disk_path=/opt/data

_cppflags_com=""
_cflags_com=""
_cxxflags_com=""
_ldflag_com=""
_param_com=""

_cppflags_com="${_cppflags_com} -W -Wall -Werror"
_cppflags_com="${_cppflags_com} -Wno-unused-parameter"
_cppflags_com="${_cppflags_com} -Wno-error=unused-but-set-variable"
_cppflags_com="${_cppflags_com} -Wno-error=unused-variable"
_cppflags_com="${_cppflags_com} -Wno-error=unused-function"
_cppflags_com="${_cppflags_com} -pipe"
_cppflags_com="${_cppflags_com} -ffunction-sections"
_cppflags_com="${_cppflags_com} -fdata-sections"
_cppflags_com="${_cppflags_com} -fstack-protector-all"
_ldflag_com="${_ldflag_com} -rdynamic"
_ldflag_com="${_ldflag_com} -Wl,--gc-sections"
_ldflag_com="${_ldflag_com} -Wl,--as-needed"

if [ x$1 = x"pc" ]; then
    vender=pc
    gcc_version=x86_64-linux-gnu
elif [ x$1 = x"arm" ]; then
    vender=hisi
    host=arm-himix200-linux
    gcc_version=arm-himix200-linux
    gcc_prefix=arm-himix200-linux
    cross_gcc_path=${data_disk_path}/opt/toolchains/${vender}/${gcc_version}/bin/${gcc_prefix}-
elif [ x$1 = x"mcu" ]; then
    vender=gnu_arm_embedded
    host=arm-none-eabi
    gcc_version=gcc-arm-none-eabi-5_4-2016q3
    gcc_prefix=arm-none-eabi
    cross_gcc_path=${data_disk_path}/opt/toolchains/${vender}/${gcc_version}/bin/${gcc_prefix}-

    _ldflag_com="${_ldflag_com} -specs=nano.specs -specs=nosys.specs"
else
    help_info
fi

# 3rd_lib path
lib_3rd_path=${data_disk_path}/install/${vender}/${gcc_version}
_cppflags_com="${_cppflags_com} -I${lib_3rd_path}/include"
_ldflag_com="${_ldflag_com} -L${lib_3rd_path}/lib"

# target
target_path=`pwd`
prefix_path=${lib_3rd_path}

cd ${target_path} && ./autogen.sh && cd -

if [ $# = 2 ]; then
    mkdir -p $2/${vender}
    cd $2/${vender}
fi

${target_path}/configure                                    \
    CC=${cross_gcc_path}gcc                                 \
    CXX=${cross_gcc_path}g++                                \
    CPPFLAGS="${_cppflags_com} "                            \
    CFLAGS="${_cflags_com}"                                 \
    CXXFLAGS="${_cxxflags_com}"                             \
    LDFLAGS="${_ldflag_com}"                                \
    LIBS=""                                                 \
    PKG_CONFIG_PATH="${lib_3rd_path}/lib/pkgconfig"         \
    --prefix=${prefix_path}                                 \
    --build=                                                \
    --host=${host}                                          \
    --target=${host}                                        \
    \
    ${_param_com}


thread_jobs=`getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1`

make -j${thread_jobs}; make install

