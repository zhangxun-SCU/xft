cmake_minimum_required(VERSION 3.27)
project(xft)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS "-static")


include_directories(xft include)

aux_source_directory(src XFT_SRC)

add_executable(xft ${XFT_SRC} main.cpp)
