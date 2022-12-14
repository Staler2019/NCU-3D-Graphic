# !/bin/bash

# ./build.sh


### Please Build before Run ###

read -p "Enter test file(in test_file/Data folder): " TESTFILE
echo "Using test file: ./Data/$TESTFILE"


# ./build/app/lab2 $TESTFILE
./build/lab3 "./$TESTFILE"
