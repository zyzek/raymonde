#pragma once

#include <forward_list>
#include <utility>
#include <vector>

#include "Vector.hpp"
#include "Sphere.hpp"
#include "Material.hpp"
#include "Camera.hpp"
#include "Ray.hpp"

struct Scene {
    Camera camera;
    Material background;
    // Pairs of spheres and their distances from the camera
    std::forward_list<std::pair<Sphere *const, float>> spheres;

    Scene(const Camera &c, const Material &b)
            : camera(c), background(b), spheres() {}

    void add_sphere(const Pos3f &position, const float &radius, const Material &material);

    void add_sphere(const Sphere &sphere);

    void remove_sphere(Sphere *sphere);

    void clear();

    void reorder();

    Vec3f raycast(const Ray3f &ray);

    void render(const size_t& width, const size_t& height, std::vector<Vec3f> &framebuffer);
};
