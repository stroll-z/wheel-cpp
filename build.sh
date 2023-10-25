#! /usr/bin/env zsh

rm -rf ./lib
mkdir -p ./lib
rm -rf ./cmake-build
mkdir -p ./cmake-build
cd ./cmake-build;cmake ..;make
