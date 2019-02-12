#pragma once

#include <forward_list>
#include <utility>
#include <vector>

#include "Camera.hpp"
#include "Material.hpp"
#include "Vector.hpp"
#include "Sphere.hpp"
#include "Light.hpp"

struct Sphere;

struct Scene {
    Camera camera;
    Vec3f background_colour;
    Vec3f ambient_colour;
    std::vector<Sphere *> spheres;
    std::vector<Light *> lights;

    Scene(const Camera &c, const Vec3f &b, const Vec3f &a)
            : camera(c), background_colour(b), ambient_colour(a), spheres(), lights() {}

    ~Scene() {
        clear();
    }

    void add_sphere(const Pos3f &position, const float &radius, const Material &material);

    void add_light(const Pos3f &position, const Vec3f &colour, const float &brightness);

    void clear();

    bool raycast(const Ray3f &ray, Sphere *&sphere_pointer, Ray3f &collision_normal) const;

    Vec3f surface_colour(const Ray3f &ray);

    void render(const size_t &width, const size_t &height, std::vector<Vec3f> &framebuffer);
};
