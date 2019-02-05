#include "Light.hpp"

/*
 * Return the light colour/intensity at a given point,
 * if there were no objects in the way.
 */
Vec3f Light::illumination(const Pos3f &pos) {
    float distance = (pos - position).length();
    return colour * brightness / (distance * distance);
}
