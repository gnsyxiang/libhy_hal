#!/bin/sh

# set -x

TOP_DIR=.
BUILD_AUX_DIR=${TOP_DIR}/res

# generate version number
generate_version_num()
{
    cd ${BUILD_AUX_DIR}
    ./get_project_info.sh
    cd -  >/dev/null 2>&1
}

get_cross_gcc_info()
{
    cd ${BUILD_AUX_DIR}
    ./get_cross_gcc_info.sh $1
    cd -  >/dev/null 2>&1
}

# use autotools to configure
autotools_configure()
{
    autoreconf -fvi -Im4
}

generate_version_num

get_cross_gcc_info $1

autotools_configure
