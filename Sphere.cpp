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
 *
 * Input vectors are assumed to be of unit length.
 */
Vec3f Sphere::surface_colour(const Ray3f &incident_ray, const Ray3f &collision_normal, const Scene &scene) const {
    Ray3f coll_normal = displace_normal_outward(collision_normal);

    //Vec3f surface_lighting = material.compute_ambient(scene.ambient_colour);
    Vec3f surface_lighting = scene.ambient_colour;
    Vec3f surface_lighting_spec(0,0,0);

    // For each light: cast a ray towards the light, checking if it hit something first.
    for (auto light : scene.lights) {
        auto illumination_ray = Ray3f(coll_normal.position, (light->position - coll_normal.position).unit());
        Ray3f shadow_collision_normal;
        Sphere *sphere_pointer;
        bool collided = scene.raycast(illumination_ray, sphere_pointer, shadow_collision_normal);

        // If the ray never collided with geometry, the light wasn't occluded.
        // If the collision point is behind the light, no occlusion.
        if (!collided ||
            distance(coll_normal.position, shadow_collision_normal.position) >
            distance(coll_normal.position, light->position)) {

            const Vec3f surface_illumination = light->illumination(collision_normal.position);
            const float diffuse_intensity = illumination_ray.direction * collision_normal.direction; // These are both unit vectors.

            // Specular component: varies with the cosine of the angle between the incident ray (camera) and the
            // direction of light reflected across the surface normal. (Brighter if reflecting directly into the camera)
            const Vec3f reflected_ray = 2 * (-illumination_ray.direction * collision_normal.direction) * collision_normal.direction + illumination_ray.direction;
            const float specular_intensity = pow(reflected_ray * incident_ray.direction, material.specularity);

            const Vec3f diffuse = hadamard(surface_illumination * diffuse_intensity, material.diffuse_colour);
            const Vec3f specular = hadamard(surface_illumination * specular_intensity, material.specular_colour);
            surface_lighting += diffuse;
            surface_lighting_spec += specular;
        }
    }

    return hadamard(surface_lighting, material.diffuse_colour) + hadamard(surface_lighting_spec, material.specular_colour);
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
