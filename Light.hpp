#pragma once

#include "Geometry.hpp"

struct Light {
    Pos3f position;
    Vec3f colour;
    float brightness;

    Light(Pos3f pos, Vec3f col, float bright)
            : position(pos), colour(col), brightness(bright) {}

    Vec3f illumination(const Pos3f &pos);
};
