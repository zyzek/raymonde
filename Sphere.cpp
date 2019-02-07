#include <iostream>

#include "constants.hpp"
#include "Sphere.hpp"

/*
 * Return true iff the given ray intersects with this sphere,
 * additionally returning the surface normal ray located at the first collision point if it exists.
 */
bool Sphere::raycast(const Ray3f &ray, Ray3f &normal) const {
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
Ray3f Sphere::displace_normal_outward(const Ray3f &normal) const {
    Vec3f centre_to_normal = normal.position - centre;
    return {centre_to_normal + centre_to_normal.unit(INCIDENT_NORMAL_DISPLACEMENT) + centre, normal.direction};
}

/*
 * Assuming a collision has occurred, pass in the ray and the collision normal
 * it induces on this sphere, and return the observed colour at that point.
 */
Vec3f Sphere::surface_colour(const Ray3f &ray, const Ray3f &collision_normal, const Scene &scene) const {
    Ray3f coll_normal = displace_normal_outward(collision_normal);

    Vec3f surface_lighting = scene.ambient_colour;

    // For each light: cast a ray towards the light, checking if it hit something first.
    for (auto light : scene.lights) {
        auto incident_ray = Ray3f(coll_normal.position, (light->position - coll_normal.position).unit());
        Ray3f incident_collision_normal;
        Sphere *sphere_pointer;
        bool collided = scene.raycast(incident_ray, sphere_pointer, incident_collision_normal);

        // If the ray never collided with geometry, the light wasn't occluded.
        // If the collision point is behind the light, no occlusion.
        if (!collided ||
            distance(coll_normal.position, incident_collision_normal.position) >
            distance(coll_normal.position, light->position)) {
            const float intensity = incident_ray.direction * coll_normal.direction; // These are both unit vectors.
            surface_lighting += light->illumination(coll_normal.position) * intensity;
        }
    }

    return hadamard(surface_lighting, material.diffuse_colour);
}

/*
 * Return the nearest distance from the given point to the sphere's surface.
 */
float Sphere::nearest_distance(const Pos3f &position) const {
    return std::abs((centre - position).length() - radius);
}

std::ostream &operator<<(std::ostream &out, const Sphere &sphere) {
    out << "Sphere(" << sphere.centre << ", " << sphere.radius << ")";
    return out;
}
