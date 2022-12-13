cmake_minimum_required(VERSION 3.10)

project(lab3)

set(CMAKE_CXX_STANDARD 17)

#includes for my libs & Lab3.cpp
#include_directories(./extern/OBJ-Loader/include ./extern/cpp-dotenv/include ./include/Graph)
# include_directories(extern/OBJ-Loader/include include/Graph)
include_directories(include/Graph)

# extern libs
# add_subdirectory(extern/OBJ-Loader) # obj-loader

#add_subdirectory(extern/cpp-dotenv) # cpp_dotenv

# my libs
add_subdirectory(src)

add_executable(lab3 apps/Lab3.cpp)

#target_link_libraries(lab3 obj-loader graph cpp_dotenv glut GLU GL)
# target_link_libraries(lab3 obj-loader graph glut GLU GL)
target_link_libraries(lab3 graph glut GLU GL)
