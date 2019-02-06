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
    // Pairs of spheres and their distances from the camera
    std::forward_list<std::pair<Sphere *const, float>> spheres;
    std::vector<Light> lights;

    Scene(const Camera &c, const Material &b)
            : camera(c), background(b), spheres(), lights() {}

    void add_sphere(const Pos3f &position, const float &radius, const Material &material);

    void add_sphere(const Sphere &sphere);

    void remove_sphere(Sphere *sphere);

    void add_light(const Pos3f &position, const Vec3f &colour, const float &brightness);

    void add_light(const Light &light);

    void clear();

    void reorder();

    bool ray_intersects(const Ray3f &ray, Ray3f &collision_normal) const;

    Vec3f raycast(const Ray3f &ray);

    void render(const size_t& width, const size_t& height, std::vector<Vec3f> &framebuffer);
};
