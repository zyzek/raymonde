#include <algorithm>

#include "Scene.hpp"

void add_by_dist(Sphere *const sphere, const Vec3f &reference, decltype(Scene::spheres) &spheres) {
    const float dist = sphere->nearest_distance(reference);
    auto p = std::make_pair(sphere, dist);

    auto prev = spheres.before_begin();
    for (auto it = spheres.begin(); it != spheres.end(); it++) {
        if (it->second > dist) {
            break;
        }
        prev = it;
    }
    spheres.insert_after(prev, p);
}

/*
 * Insert a new sphere, ordering by distance from the camera.
 * Efficiency is worst/expected case linear for a single insertion,
 * thus quadratic for a sequence; not great.
 */
void Scene::add_sphere(const Vec3f &position, const float &radius, const Material &material) {
    auto sphere = new Sphere(position, radius, material);
    add_by_dist(sphere, camera.position, spheres);
}

void Scene::add_sphere(const Sphere &sphere) {
    add_sphere(sphere.centre, sphere.radius, sphere.material);
}


/*
 * Remove and destroy the given sphere.
 */
void Scene::remove_sphere(Sphere *sphere) {
    auto comparator = [sphere](const std::pair<Sphere *const, float> &p) -> bool { return p.first == sphere; };
    auto it = std::find_if(spheres.begin(), spheres.end(), comparator);

    if (it != spheres.end()) {
        spheres.remove(*it);
        delete it->first;
    }
}

/*
 * All spheres are destroyed.
 */
void Scene::clear() {
    for (auto sphere : spheres) {
        delete sphere.first;
    }
    spheres.clear();
}

/*
 * Reorder the spheres in the current scene relative to the camera.
 * This is useful if the camera position changes.
 */
void Scene::reorder() {
    auto new_spheres = spheres;
    spheres.clear();
    for (auto sphere : new_spheres) {
        add_by_dist(sphere.first, camera.position, spheres);
    }
}

/*
 * Return the colour of the ray if it collides with anything,
 * otherwise return the background colour.
 */
Vec3f Scene::raycast(const Ray3f &ray) {
    Vec3f colour = this->background.diffuse_colour;
    for (auto sphere : spheres) {
        if (sphere.first->raycast(ray, colour)) {
            break;
        }
    }
    return colour;
}

/*
 * Given a width, a height, and a buffer to render to,
 * fill the buffer with an image of the scene.
 */
void Scene::render(const size_t &width, const size_t &height, std::vector<Vec3f> &framebuffer) {
    const float x_fov_tan = std::tan(camera.fov / 2.0f);
    const float y_fov_tan = x_fov_tan * (float) height / (float) width;
    const float x_comp = 2.0f * camera.plane_distance * x_fov_tan / ((float) width - 1);
    const float y_comp = -2.0f * camera.plane_distance * y_fov_tan / ((float) height - 1);

#pragma omp parallel for
    for (ssize_t j = 0; j < height; j++) {
        const float y = (j - height / 2.0f) * y_comp;
        for (ssize_t i = 0; i < width; i++) {
            const float x = (i - width / 2.0f) * x_comp;
            Ray3f ray = Ray3f(camera.position, Vec3f(x, y, camera.plane_distance));
            framebuffer[i + j * width] = raycast(ray);
        }
    }
}


