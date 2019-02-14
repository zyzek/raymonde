#pragma once

#include "Geometry.hpp"
#include "Material.hpp"
#include "Scene.hpp"

struct Scene;

struct SinePlane {
    Vec2f frequency;
    Vec3f offset;
    Material material;

    SinePlane(const Vec2f &f, const Vec3f &o, const Material &m)
            : frequency(f), offset(o), material(m) {}

    bool raycast(const Ray3f &ray, Ray3f &normal) const;

    Vec3f surface_colour(const Ray3f &incident_ray, const Ray3f &collision_normal, const Scene &scene) const;

private:

    Ray3f displace_normal_outward(const Ray3f &normal) const;

};


