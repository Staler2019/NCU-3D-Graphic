cmake_minimum_required(VERSION 3.10)

project(graph)

add_library(graph BasicGraph.cpp Circle.cpp Line.cpp Point.cpp Poly.cpp RGBCode.cpp)
# target_link_libraries(graph glut GLU GL)
