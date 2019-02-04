#pragma once

#include "Vector.hpp"

struct Material {
    Vec3f diffuse_colour;

    Material() : diffuse_colour(1.0, 0.0, 1.0) {}

    explicit Material(const Vec3f &diffuse) : diffuse_colour(diffuse) {}
};
