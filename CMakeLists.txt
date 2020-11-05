cmake_minimum_required(VERSION 3.17)
project(CLion)

set(CMAKE_CXX_STANDARD 20)

add_executable(CLion main.cpp taskreader.h taskreader.cpp)