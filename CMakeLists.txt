cmake_minimum_required(VERSION 3.7)
project(lab6)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp color.h glut.h point.h window.h list.h node.h link.h algVA.h pixel.h)
add_executable(lab6 ${SOURCE_FILES})

target_link_libraries(lab6 GL glut GLU)