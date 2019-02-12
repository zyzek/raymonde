#include <algorithm>
#include <limits>

#include "Scene.hpp"

/*
 * Insert a new sphere.
 */
void Scene::add_sphere(const Pos3f &position, const float &radius, const Material &material) {
    auto *sphere = new Sphere(position, radius, material);
    spheres.push_back(sphere);
}

/*
 * Add a new light to the scene
 */
void Scene::add_light(const Pos3f &position, const Vec3f &colour, const float &brightness) {
    auto *light = new Light(position, colour, brightness);
    lights.push_back(light);
}

/*
 * All spheres and lights are destroyed.
 */
void Scene::clear() {
    for (auto sphere : spheres) {
        delete sphere;
    }
    spheres.clear();

    for (auto light : lights) {
        delete light;
    }
    lights.clear();
}

/*
 * Return true iff the given ray intersects with any sphere in the scene,
 * returning. Additionally return:
 *  * A pointer to the sphere which was hit, if any.
 *  * A ray located at the first collision point if it exists,
 *    normal to the surface at that point.
 *
 * Iterates though all objects in the scene to check for collisions,
 * which is not particularly efficient.
 */
bool Scene::raycast(const Ray3f &ray, Sphere *&sphere_pointer, Ray3f &collision_normal) const {
    float dist = std::numeric_limits<float>::max();
    Ray3f current_collision_normal;
    bool collided = false;

    for (auto sphere : spheres) {
        const bool current_collided = sphere->raycast(ray, current_collision_normal);
        if (current_collided) {
            collided = true;
            const float current_dist = distance(ray.position, current_collision_normal.position);
            if (current_dist < dist) {
                dist = current_dist;
                collision_normal = current_collision_normal;
                sphere_pointer = sphere;
            }
        }
    }

    return collided;
}

/*
 * Return the colour of the ray if it collides with anything,
 * otherwise return the background colour.
 */
Vec3f Scene::surface_colour(const Ray3f &ray) {
    Sphere *sphere_pointer;
    Ray3f collision_normal;
    if (raycast(ray, sphere_pointer, collision_normal)) {
        return sphere_pointer->surface_colour(ray, collision_normal, *this);
    };
    return this->background_colour;
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
            Ray3f ray = Ray3f(camera.position, Vec3f(x, y, camera.plane_distance).unit());
            framebuffer[i + j * width] = surface_colour(ray);
        }
    }
}


