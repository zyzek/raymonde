#pragma once

#include "Material.hpp"
#include "Ray.hpp"

struct Sphere {
    Pos3f centre;
    float radius;
    Material material;

    Sphere(const Pos3f &c, const float &r, const Material &m)
            : centre(c), radius(r), material(m) {}

    bool ray_intersects(const Ray3f &ray, Ray3f &normal) const;

    bool raycast(const Ray3f& ray, Vec3f &colour) const;

    float nearest_distance(const Pos3f &position) const;

};

