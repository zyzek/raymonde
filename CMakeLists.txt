cmake_minimum_required(VERSION 3.13)
project(raymonde)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(raymonde
        main.cpp
        Material.hpp
        README.md
        Sphere.hpp
        Vector.hpp Scene.cpp Scene.hpp Camera.hpp constants.hpp Sphere.cpp Material.cpp Light.hpp Light.cpp)
