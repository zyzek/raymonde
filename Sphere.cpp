#include <iostream>

#include "Sphere.hpp"
#include "constants.hpp"

/*
 * Return whether the given ray intersects with this sphere,
 * returning the distance between the ray's position and the
 * first intersection point into dist if it exists.
 */
bool Sphere::ray_intersects(const Ray3f &ray, Ray3f &normal) const {
    const Vec3f to_sphere = centre - ray.position;

    // Compute the projection of the sphere's position vector onto the ray.
    const Vec3f unit_ray = ray.direction.unit();
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
    if (semi_dist < MERGE_EPSILON || p_dist < radius) {
        normal.position = (p.length() + semi_dist) * unit_ray + ray.position;
    } else {
        normal.position = (p.length() - semi_dist) * unit_ray + ray.position;
    }
    normal.direction = (normal.position - centre).unit();
    return true;
}

/*
 * Cast a ray from an origin, returning the whether the collision
 * occurred, and colour of the first collision point with the sphere,
 * if it exists.
 */
bool Sphere::raycast(const Ray3f &ray, Vec3f &colour) const {
    Ray3f normal;
    if (!ray_intersects(ray, normal)) {
        return false;
    }
    const float intensity = ray.direction.unit() * normal.direction.unit();
    colour = material.diffuse_colour * intensity;
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
