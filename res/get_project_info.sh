#!/usr/bin/env bash

# set -x

TOP_DIR=..
m4_dir=${TOP_DIR}/m4
check_project_info=${m4_dir}/check_project_info.m4

get_project_name()
{
    libproject_name=$1
    libproject_url=$2

    project_name=${libproject_name#*lib}
    project_name_2=${libproject_name%lib*}
    echo -e "\e[32m\tproject_name: $libproject_name \e[0m"

    mkdir -p ${m4_dir}

    echo    "m4_define([PROJECT_NAME], [$libproject_name])" >   ${check_project_info}
    echo    "m4_define([CHECK_PROJECT_INFO],"               >>  ${check_project_info}
    echo -e "\t\t  ["                                       >>  ${check_project_info}
    if [ x$project_name_2 = x ]; then
        echo -e "\t\t\tproject_name=$project_name"          >>  ${check_project_info}
    else
        echo -e "\t\t\tproject_name=$libproject_name"       >>  ${check_project_info}
    fi
    echo -e "\t\t\tAC_SUBST(project_name)"                  >>  ${check_project_info}
    echo -e "\t\t  ])"                                      >>  ${check_project_info}
    echo    "m4_define([EMAIL_INFO], [gnsyxiang@163.com])"  >>  ${check_project_info}
    echo    "m4_define([PROJECT_URL], [$libproject_url])"   >>  ${check_project_info}
}

get_full_version_num()
{
    full_version=$1

    current=`echo $full_version    | awk -F '_' '{print $1}' | awk -F '.' '{print $1}'`
    revision=`echo $full_version   | awk -F '_' '{print $1}' | awk -F '.' '{print $2}'`
    age=`echo $full_version        | awk -F '_' '{print $1}' | awk -F '.' '{print $3}'`

    echo -e "\e[32m\tfull_version: $full_version \e[0m"

    echo    "m4_define([VERSION_NUM], [$full_version])"     >>  ${check_project_info}
    echo    "m4_define([CHECK_VERSION_INFO],"               >>  ${check_project_info}
    echo -e "\t\t  ["                                       >>  ${check_project_info}
    echo -e "\t\t\tcurrent=$current"                        >>  ${check_project_info}
    echo -e "\t\t\trevision=$revision"                      >>  ${check_project_info}
    echo -e "\t\t\tage=$age"                                >>  ${check_project_info}
    echo -e "\t\t\tAC_SUBST(current)"                       >>  ${check_project_info}
    echo -e "\t\t\tAC_SUBST(revision)"                      >>  ${check_project_info}
    echo -e "\t\t\tAC_SUBST(age)"                           >>  ${check_project_info}
    echo -e "\t\t  ])"                                      >>  ${check_project_info}
}



if [ -d $TOP_DIR/.git ]; then
    echo "### get project name..."

    libproject_url=`git remote -v | head -n1 | awk '{print $2}' | sed 's/git@/https:\/\//' | sed 's/com:/com\//'`
    libproject_name=`git remote -v | head -n1 | awk '{print $2}' | sed 's/.*\///' | sed 's/\.git//'`

    get_project_name $libproject_name $libproject_url

    echo "### get version num..."
    libproject_version=`git describe --tags | sed s/-/_/g`

    get_full_version_num $libproject_version
else
    echo "### get project name..."
    project_path=`pwd`
    project_path_left=${project_path%/*}
    project_path_right=${project_path_left##*/}

    project_name=${project_path_right%-*}

    get_project_name $project_name

    echo "### get version num..."
    project_version=${project_path_right#*-}

    get_full_version_num $project_version
fi


