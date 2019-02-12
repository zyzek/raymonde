#pragma once

#include "Geometry.hpp"
#include "Material.hpp"
#include "Scene.hpp"

struct Scene;

struct Sphere {
    Pos3f centre;
    float radius;
    Material material;

    Sphere(const Pos3f &c, const float &r, const Material &m)
            : centre(c), radius(r), material(m) {}

    bool raycast(const Ray3f &ray, Ray3f &normal) const;

    Vec3f surface_colour(const Ray3f &incident_ray, const Ray3f &collision_normal, const Scene &scene) const;

    float nearest_distance(const Pos3f &position) const;

private:

    Ray3f displace_normal_outward(const Ray3f &normal) const;

};

