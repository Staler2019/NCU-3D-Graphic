cmake_minimum_required(VERSION 3.10)

# include_directories(${CMAKE_SOURCE_DIR})

project(lab2)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab2 Lab2.cpp)

add_subdirectory(Graph)

target_link_libraries(lab2 graph glut GLU GL)

