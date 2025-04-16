#!/bin/bash
#
#  Look for makefiles in subdirectories and call make
#
BASE_DIR=$(dirname ${BASH_SOURCE[0]})
BASE_DIR=$(realpath ${BASE_DIR})
MAKEFILES=$(find ${BASE_DIR} -type f -name makefile)
FINAL_RESULT=0

((THREADS=$(nproc)/2)); 
echo Build threads: $THREADS


echo "Base dir: ${BASE_DIR}"
cd $BASE_DIR

echo "--------> Building ramulator2"
mkdir ext/ramulator2/build
cd ext/ramulator2/build
cmake ..
make -j $THREADS
cp ./ramulator2 ../ramulator2             


echo "--------> Building GEM5"
cd $BASE_DIR
scons build/RISCV/gem5.opt -j $THREADS

