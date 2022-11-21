# !/bin/bash

./build.sh


### Please Build before Run ###

read -p "Enter test file(in test/Data folder): " TESTFILE
echo "Using test file: ./build/test/Data/$TESTFILE"


# ./build/app/lab2 $TESTFILE
./build/app/lab3 "./build/test/Data/$TESTFILE"
