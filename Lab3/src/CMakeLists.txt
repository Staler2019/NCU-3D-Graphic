cmake_minimum_required(VERSION 3.10)

project(graph)

set(graph-headers
    "${CMAKE_SOURCE_DIR}/include/Graph/Camera.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/GMath.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/GObj.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/GRGB.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/GShape.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Scene.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Transform.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Math/Matrix.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Math/Vector.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Math/Util.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Shape/Circle.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Shape/Line.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Shape/Point.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Shape/Poly.h"
    "${CMAKE_SOURCE_DIR}/include/Graph/Shape/Shape.h"
)

set(graph-src-files
    Math/Matrix.cpp
    Math/Vector.cpp
    Shape/Line.cpp
    Shape/Point.cpp
    Shape/Poly.cpp
    Shape/Circle.cpp
    Camera.cpp
    GObj.cpp
    Scene.cpp
    Transform.cpp
)

# Make an automatic library - will be static or dynamic based on user setting
add_library(graph ${graph-src-files} ${graph-headers})

# We need this directory, and users of our library will need it too
# include_directories(graph PUBLIC ./include/Graph)

# IDEs should put the headers in a nice place
source_group(
  TREE "${PROJECT_SOURCE_DIR}/include/Graph"
  PREFIX "Header Files"
  FILES ${HEADER_LIST})
