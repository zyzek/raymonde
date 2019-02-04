cmake_minimum_required(VERSION 3.13)
project(raymonde)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(raymonde
        main.cpp
        material.hpp
        README.md
        sphere.hpp
        vector.hpp)
