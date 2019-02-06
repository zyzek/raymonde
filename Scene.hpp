#pragma once

#include <forward_list>
#include <utility>
#include <vector>

#include "Vector.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "Camera.hpp"
#include "Ray.hpp"
#include "Light.hpp"

struct Sphere;

struct Scene {
    Camera camera;
    Material background;
    std::vector<Sphere *> spheres;
    std::vector<Light> lights;

    Scene(const Camera &c, const Material &b)
            : camera(c), background(b), spheres(), lights() {}

    void add_sphere(const Pos3f &position, const float &radius, const Material &material);

    void add_light(const Pos3f &position, const Vec3f &colour, const float &brightness);

    void clear();

    bool raycast(const Ray3f &ray, Sphere *&sphere_pointer, Ray3f &collision_normal) const;

    Vec3f surface_colour(const Ray3f &ray);

    void render(const size_t& width, const size_t& height, std::vector<Vec3f> &framebuffer);
};
