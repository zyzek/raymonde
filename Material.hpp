#pragma once

#include "Vector.hpp"

struct Material {
    Vec3f diffuse_colour;
    float specularity;
    float shininess;

    Material() : diffuse_colour(1.0, 0.0, 1.0), specularity(0.1), shininess(1.0) {}

    explicit Material(const Vec3f &diffuse, const float &spec, const float &shine)
            : diffuse_colour(diffuse), specularity(spec), shininess(shine) {}
};
