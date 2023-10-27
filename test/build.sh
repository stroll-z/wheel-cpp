#! /usr/bin/env zsh

if [ ! $# -eq 1 ];then
echo -e "\e[31m需要输入想要编译的sample路径\e[m"
echo -e "\e[31m我是一个例子:./build_sample.sh cipher/aes_demo.c\e[m"
exit -1
fi

ROOT_DIR=$PWD

# echo "$1"
touch "$1"
mkdir -p cmake-build;
SAMPLE_SRC="$1"
VERBOSE=1
# export VERBOSE

export SAMPLE_SRC

# 先编译库
cd $ROOT_DIR/../;./build.sh;cd $ROOT_DIR
cd cmake-build;cmake ..;make -j8;cd -;chmod +x ./demo
