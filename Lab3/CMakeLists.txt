cmake_minimum_required(VERSION 3.10)

project(lab3)

set(CMAKE_CXX_STANDARD 17)

#includes for my libs & Lab3.cpp
include_directories(include/Graph)

# my libs
add_subdirectory(src)

add_executable(lab3 apps/Lab3.cpp)

target_link_libraries(lab3 graph glut GLU GL)
