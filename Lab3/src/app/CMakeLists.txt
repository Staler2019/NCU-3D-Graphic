cmake_minimum_required(VERSION 3.10)

include_directories(${CMAKE_SOURCE_DIR})

project(lab3)

set(CMAKE_CXX_STANDARD 17)

add_executable(lab3 Lab3.cpp)

if(UNIX)
    target_link_libraries(lab3 obj-loader graph glut GLU GL)
endif(UNIX)

if(MINGW)
    target_link_libraries(lab3 obj-loader graph opengl32 glew32 freeglut glu32)
endif(MINGW)
