cmake_minimum_required(VERSION 3.10)

include_directories(${CMAKE_SOURCE_DIR})

project(lab2)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab2 lab2.cpp)
target_link_libraries(lab2 graph glut GLU GL)
