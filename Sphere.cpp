#include <iostream>

#include "constants.hpp"
#include "Sphere.hpp"

/*
 * Return whether the given ray intersects with this sphere,
 * returning the ray located at the first collision point if it exists,
 * normal to the surface at that point.
 */
bool Sphere::ray_intersection(const Ray3f &ray, Ray3f &normal) const {
    const Vec3f to_sphere = centre - ray.position;

    // If the whole sphere is behind the ray, there is no intersection point.
    if (to_sphere * ray.direction < 0 &&
        distance(ray.position, centre) > radius) {
        return false;
    }

    // Compute the projection of the sphere's position vector onto the ray.
    const Vec3f p = projection(to_sphere, ray.direction);
    const float p_dist = (p - to_sphere).length();

    // There are no intersections if the length of the projected vector
    // is greater than the radius.
    if (p_dist > radius) {
        return false;
    }

    // If the intersection points coincide, or the ray originates within the sphere,
    // then there is only one intersection point; return the distance to the furthest one.
    // Otherwise use the nearest.
    const float semi_dist = std::sqrt(radius * radius - p_dist * p_dist);
    if (semi_dist < MERGE_EPSILON || to_sphere.length() < radius) {
        normal.position = (p.length() + semi_dist) * ray.direction.unit() + ray.position;
    } else {
        normal.position = (p.length() - semi_dist) * ray.direction.unit() + ray.position;
    }
    normal.direction = (normal.position - centre).unit();
    return true;
}

/*
 * Move a ray slightly outwards from the origin.
 * This is used to displace collision normals so tht spheres do not self-occlude.
 */
void Sphere::displace_normal_outward(Ray3f &normal) const {
    Vec3f centre_to_normal = normal.position - centre;
    normal.position = centre_to_normal + centre_to_normal.unit(INCIDENT_NORMAL_DISPLACEMENT) + centre;
}

/*
 * Cast a ray from an origin, returning the whether the collision
 * occurred, and colour of the first collision point with the sphere,
 * if it exists.
 */
bool Sphere::raycast(const Ray3f &ray, Vec3f &colour, const Scene &scene) const {
    Ray3f collision_normal;
    if (!ray_intersection(ray, collision_normal)) {
        return false;
    }
    displace_normal_outward(collision_normal);

    Vec3f surface_lighting(0, 0, 0);

    // For each light: cast a ray towards the light, checking if it hit something first.
    for (auto light : scene.lights) {
        auto incident_ray = Ray3f(collision_normal.position, (light.position - collision_normal.position).unit());
        Ray3f incident_collision_normal;
        Sphere *sphere_pointer;
        bool collided = scene.ray_intersection(incident_ray, sphere_pointer, incident_collision_normal);

        // If the ray never collided with geometry, the light wasn't occluded.
        // If the collision point is behind the light, no occlusion.
        if (!collided ||
            distance(collision_normal.position, incident_collision_normal.position) > distance(collision_normal.position, light.position)) {
            const float intensity = incident_ray.direction * collision_normal.direction; // These are both unit vectors.
            surface_lighting = surface_lighting + (light.illumination(collision_normal.position) * intensity);
        }
    }

    colour = hadamard(surface_lighting, material.diffuse_colour);
    return true;
}

/*
 * Return the nearest distance from the given point to the sphere's surface.
 */
float Sphere::nearest_distance(const Pos3f &position) const {
    const float dist = (centre - position).length();
    return dist > radius ? dist - radius : radius - dist;
}

std::ostream &operator<<(std::ostream &out, const Sphere &sphere) {
    out << "Sphere(";
    out << sphere.centre << ", ";
    out << sphere.radius << ", ";
    out << ")";
    return out;
}
