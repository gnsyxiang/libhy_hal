# README

## 依赖

* `autotools`
* `java`
* `doxygen`
* `graphviz`

### autotools

需要安装autotools工具 (`sudo apt install -y autoconf automake`)

### java

因为使用了`*.plantuml`脚本开发相关时序图，该脚本转化为svg图片的过程中需要用到java开发环境

### doxygen

如果需要生成html文档，需要安装doxygen软件，把相关注释转化成文档 (`sudo apt install -y doxygen`)

### graphviz

如果需要生成html文档中的调用图，需要安装graphviz软件，把函数调用通过图形的方式展示出来，并可以通过图形跳转 (`sudo apt install -y graphviz`)

## 目录说明

### 目录

```txt
.
├── autogen.sh
├── build-script                    # 配置厂商，芯片，产品，语言等相关信息
│   ├── arterytek
│   │   ├── at32f4xx
│   │   │   ├── config.sh           # 配置脚本
│   │   │   └── Makefile.am
│   │   └── Makefile.am
│   ├── eeasytech
│   │   ├── Makefile.am
│   │   └── SV823
│   │       ├── config.sh           # 配置脚本
│   │       └── Makefile.am
│   ├── Makefile.am
│   └── pc
│       ├── Makefile.am
│       └── pc-chip
│           ├── config.sh           # 配置脚本
│           └── Makefile.am
├── build.sh
├── configure.ac
├── doc
├── example                         # 测试demo
├── hy_hal.pc.in
├── include                         # 头文件
├── LICENSE
├── m4
├── Makefile.am
├── README.md
├── res
└── src                             # 源文件
```

### 配置脚本

```txt
#!/usr/bin/env bash

chip=pc-chip                        # 芯片型号

product=HY-pc                       # 支持产品

language=cn en                      # 支持语言

host=
cross_gcc_path=                     # gcc路劲

configure_param=                    # configure配置参数

cppflag=-fstack-protector-all
cflag=
cxxflag=
ldflag=-rdynamic
lib=

install_path=                       # 安装路径
```

## 使用说明

### 配置

* 执行`./build.sh`，生成`configure`脚本

```shell
$ ./build.sh 
support vender:             # 选择厂商
    eeasytech
    pc
    arterytek
please select vender: pc
support chip:               # 选择芯片
    pc-chip
please select chip: pc-chip
support build version:     # 选择是否调试版本
    release
    debug
please select build version: debug
```

* 三部曲编译源码并生成对应文件

```shell
$ ./configure
$ make
$ make install
```

## 配置相关选项

* `--prefix=PREFIX`: 指定安装路径

* `--enable-doxygen-doc`: 生成html帮助文档

