cmake_minimum_required(VERSION 3.10)

project(lab4)

set(CMAKE_CXX_STANDARD 17)

#includes for my libs & Lab3.cpp
include_directories(include/Graph)

# my libs
add_subdirectory(src)

add_executable(lab4 apps/Lab4.cpp)

target_link_libraries(lab4 graph glut GLU GL)
