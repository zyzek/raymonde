#pragma once

#include "Vector.hpp"

struct Material {
    Vec3f diffuse_colour;
    Vec3f specular_colour;
    float specularity;

    Material() : diffuse_colour(1.0, 0.0, 1.0), specular_colour(0.0, 1.0, 0.0), specularity(1.0) {}

    explicit Material(const Vec3f &diffuse_col, const Vec3f &specular_col, const float &spec)
            : diffuse_colour(diffuse_col), specular_colour(specular_col), specularity(spec) {}
};
