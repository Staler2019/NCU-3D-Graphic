# !/bin/bash
mkdir -p build
cd build

cmake ..
make -j4

rm -rf ./Data/
cp -R ../Data/ ./Data/

rm -rf ./Mesh/
cp -R ../Mesh/ ./Mesh/
