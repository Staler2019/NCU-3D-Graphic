cmake_minimum_required(VERSION 3.10)

project(obj-loader)

set(obj-loader-src-files OBJ_Loader.h)

add_library(obj-loader ${obj-loader-src-files})
