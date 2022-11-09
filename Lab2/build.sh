# !/bin/bash
mkdir -p build
cd build

cmake ..
make

rm -rf ./test/
cp -R ../test/ ./test/
