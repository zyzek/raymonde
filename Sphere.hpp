#pragma once

#include "Material.hpp"
#include "Ray.hpp"

struct Sphere {
    Vec3f centre;
    float radius;
    Material material;

    Sphere(const Vec3f &c, const float &r, const Material &m)
            : centre(c), radius(r), material(m) {}

    bool ray_intersects(const Ray3f &ray, float &dist) const;

    bool raycast(const Ray3f& ray, Vec3f &colour) const;

    float nearest_distance(const Vec3f &position) const;
};

