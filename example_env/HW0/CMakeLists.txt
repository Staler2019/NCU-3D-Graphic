cmake_minimum_required(VERSION 3.10)
project(HW0)

set(CMAKE_CXX_STANDARD 17)

add_executable(HW0 hw0.cpp)

target_link_libraries(HW0 glut GLU GL)
