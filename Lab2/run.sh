# !/bin/bash

# ./build.sh
### Please Build before Run ###

# read -p "Enter test file: " TESTFILE
read -p "Enter test file(in test folder): " TESTFILE
echo "Using test file: ./build/test/$TESTFILE"

# ./build/app/lab2 $TESTFILE
./build/lab2 "./build/test/$TESTFILE"
