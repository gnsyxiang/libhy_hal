#!/usr/bin/env bash

# set -x

format_str_array_output()
{
    _out_put_array=(`echo ${1}`)
    for i in ${_out_put_array[@]}; do
        echo "    $i"
    done
}

select_vender()
{
    echo "support vender: "
    for i in `find ./build-script/ -maxdepth 1 -type d`; do
        _vender="${_vender} ${i##*/}"
    done

    format_str_array_output "${_vender}"

    echo -n "please select vender: "
    read usr_select_vender

    _flag="false"
    for i in `find ./build-script/ -maxdepth 1 -type d`; do
        if [[ ${i##*/} = ${usr_select_vender} ]]; then
            _flag="true"
            break
        fi
    done
    if [[ ${_flag} != "true" ]]; then
        echo "error select vender !!!"
        exit
    fi

    configure_param="${configure_param} --with-vender=${usr_select_vender}"
}

select_chip()
{
    echo "support chip: "
    for i in `find ./build-script/${usr_select_vender} -name "config.sh"`; do
        _chip=`sed '/^chip=/!d;s/.*=//' $i`
        chip="${chip} ${_chip}"
    done

    format_str_array_output "${chip}"

    echo -n "please select chip: "
    read usr_select_chip

    _flag="false"
    for i in `find ./build-script/${usr_select_vender} -name "config.sh"`; do
        _chip=`sed '/^chip=/!d;s/.*=//' $i`
        if [[ ${_chip} = ${usr_select_chip} ]]; then
            _flag="true"
            break
        fi
    done
    if [[ ${_flag} != "true" ]]; then
        echo "error select chip !!!"
        exit
    fi

    configure_param="${configure_param} --with-chip=${usr_select_chip}"
}

select_product()
{
    echo "support product: "
    _product_file=./build-script/${usr_select_vender}/${usr_select_chip}/config.sh

    _product=`sed '/^product=/!d;s/.*=//' $_product_file`

    format_str_array_output "${_product}"

    echo -n "please select product: "
    read usr_select_product

    _flag="false"
    _product_array=(`echo ${_product}`)
    for i in ${_product_array[@]}; do
        if [[ $i = ${usr_select_product} ]]; then
            _flag="true"
            break
        fi
    done
    if [[ ${_flag} != "true" ]]; then
        echo "error select product !!!"
        exit
    fi

    configure_param="${configure_param} --with-product=${usr_select_product}"
}

select_language()
{
    echo "support language: "
    _product_file=./build-script/${usr_select_vender}/${usr_select_chip}/config.sh

    _language=`sed '/^language=/!d;s/.*=//' $_product_file`

    format_str_array_output "${_language}"

    echo -n "please select language: "
    read usr_select_language

    _flag="false"
    _language_array=(`echo ${_language}`)
    for i in ${_language_array[@]}; do
        if [[ $i = ${usr_select_language} ]]; then
            _flag="true"
            break
        fi
    done
    if [[ ${_flag} != "true" ]]; then
        echo "error select language !!!"
        exit
    fi

    configure_param="${configure_param} --with-language=${usr_select_language}"
}

select_build_version()
{
    echo "support build version: "
    _build_version="release debug"

    format_str_array_output "${_build_version}"

    echo -n "please select build version: "
    read usr_select_build_version

    _flag="false"
    _product_array=(`echo "${_build_version}"`)
    for i in ${_product_array[@]}; do
        if [[ $i = ${usr_select_build_version} ]]; then
            _flag="true"
            break
        fi
    done
    if [[ ${_flag} != "true" ]]; then
        echo "error select build version !!!"
        exit
    fi

    if [[ ${usr_select_build_version} = "debug" ]]; then
        cppflag="${cppflag} -g -O0"
        configure_param="${configure_param} --enable-debug_info"
    else
        cppflag="${cppflag} -O2 -DNDEBUG"
    fi
}

get_com_config()
{
    cppflag="${cppflag} -W -Wall -Werror"
    cppflag="${cppflag} -Wno-address"
    cppflag="${cppflag} -Wno-unused-parameter"
    cppflag="${cppflag} -Wno-error=unused-but-set-variable"
    cppflag="${cppflag} -Wno-error=unused-variable"
    cppflag="${cppflag} -Wno-error=unused-function"
    cppflag="${cppflag} -pipe"
    cppflag="${cppflag} -ffunction-sections"
    cppflag="${cppflag} -fdata-sections"

    ldflag="${ldflag} -Wl,--gc-sections"
    ldflag="${ldflag} -Wl,--as-needed"
    ldflag="${ldflag} -Wl,-rpath=../lib"
}

get_config()
{
    _config_file=./build-script/${usr_select_vender}/${usr_select_chip}/config.sh

    host=`sed '/^host=/!d;s/.*=//' $_config_file`
    gcc_version=`sed '/^gcc_version=/!d;s/.*=//' $_config_file`
    cross_gcc_path=`sed '/^cross_gcc_path=/!d;s/.*=//' $_config_file`

    _cppflag=`sed '/^cppflag=/!d;s/cppflag=//' $_config_file`
    _cflag=`sed '/^cflag=/!d;s/cflag=//' $_config_file`
    _cxxflag=`sed '/^cxxflag=/!d;s/cxxflag=//' $_config_file`
    _ldflag=`sed '/^ldflag=/!d;s/ldflag=//' $_config_file`
    _lib=`sed '/^lib=/!d;s/lib=//' $_config_file`

    install_path=`sed '/^install_path=/!d;s/.*=//' $_config_file`

    cppflag="${cppflag} ${_cppflag}"
    cflag="${cflag} ${_cflag}"
    cxxflag="${cxxflag} ${_cxxflag}"
    ldflag="${ldflag} ${_ldflag}"
    lib="${lib} ${_lib}"

    echo ${lib}
}

select_vender
select_chip
# select_product
# select_language
select_build_version
get_com_config
get_config

cur_path=`pwd`

cppflag="${cppflag} -I${install_path}/include"
ldflag="${ldflag} -L${install_path}/lib"

make distclean

cd ${cur_path} && ./autogen.sh && cd -

export STRIP=${cross_gcc_path}strip
${cur_path}/configure                                       \
    CC=${cross_gcc_path}gcc                                 \
    CXX=${cross_gcc_path}g++                                \
    CPPFLAGS="${cppflag}"                                   \
    CFLAGS="${cflag}"                                       \
    CXXFLAGS="${cxxflag}"                                   \
    LDFLAGS="${ldflag}"                                     \
    LIBS="${lib}"                                           \
    PKG_CONFIG_PATH="${install_path}/lib/pkgconfig"         \
    --prefix=${install_path}                                \
    --build=                                                \
    --host=${host}                                          \
    --target=${host}                                        \
    \
    ${configure_param}

thread_jobs=`getconf _NPROCESSORS_ONLN 2>/dev/null || echo 1`

if [ x$usr_select_build_version = x"release" ]; then
    make -j${thread_jobs} && make install-strip
else
    make -j${thread_jobs} && make install
fi

