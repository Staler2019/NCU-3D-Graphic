# !/bin/bash
mkdir -p build
cd build

cmake ../src
make

./app/lab2