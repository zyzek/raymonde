cmake_minimum_required(VERSION 3.13)
project(raymonde)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(raymonde
        main.cpp
        constants.hpp
        Geometry.hpp
        Camera.hpp
        Light.hpp Light.cpp
        Material.hpp Material.cpp
        Sphere.hpp Sphere.cpp
        Scene.hpp Scene.cpp
        )
