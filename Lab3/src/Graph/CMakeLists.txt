cmake_minimum_required(VERSION 3.10)

project(graph)

set(graph-src-files
    BasicGraph.cpp BasicGraph.h
    Circle.cpp Circle.h
    Line.cpp Line.h
    Point.cpp Point.h
    Poly.cpp Poly.h
    RGBCode.cpp RGBCode.h
    Scene.cpp Scene.h
    TransformMatrix.cpp TransformMatrix.h
    Util.cpp Util.h
)

add_library(graph ${graph-src-files})
