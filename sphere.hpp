#include "material.hpp"

// Points closer than this distance are considered the same point.
#define MERGE_EPSILON 0.00001

struct Sphere {
    Vec3f centre;
    float radius;
    Material material;

    Sphere(const Vec3f &c, const float &r, const Material &m)
            : centre(c), radius(r), material(m) {}

    /*
     * Return whether the given ray cast from the given origin intersects with this sphere,
     * returning the distance between the origin and the first ray-sphere intersection point if it exists.
     */
    bool ray_intersects(const Vec3f &origin, const Vec3f &ray, float &dist) const {
        const Vec3f to_sphere = centre - origin;

        // Compute the projection of the sphere's position vector onto the ray.
        const Vec3f unit_ray = ray.normalised();
        const Vec3f p = projection(to_sphere, ray);
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
            dist = (((p.length() + semi_dist) * unit_ray) - origin).length();
        } else {
            dist = (((p.length() - semi_dist) * unit_ray) - origin).length();
        }
        return true;
    }
};

