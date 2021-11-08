#!/usr/bin/env bash

# set -x

TOP_DIR=..
m4_dir=${TOP_DIR}/m4
m4_file=${m4_dir}/get_cross_gcc_info.m4

echo    "m4_define([GCC_PREFIX], [$1])"     >   ${m4_file}
echo    "m4_define([GCC_PREFIX_INFO],"      >>  ${m4_file}
echo -e "\t["                               >>  ${m4_file}
echo -e "\t\tgcc_prefix=$1"                 >>  ${m4_file}
echo -e "\t\tAC_SUBST(gcc_prefix)"          >>  ${m4_file}
echo -e "\t\t])"                            >>  ${m4_file}

