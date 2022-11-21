# !/bin/bash
mkdir -p build
cd build

cmake ../src
make

rm -rf ./test/
cp -R ../test/ ./test/
