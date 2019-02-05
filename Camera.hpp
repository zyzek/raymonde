#pragma once

#include "Vector.hpp"
#include "constants.hpp"

struct Camera {
    Pos3f position;
    Vec3f orientation; // The position the camera is looking at relative to the origin
    float fov;
    float plane_distance;

    explicit Camera(const Pos3f &p)
            : position(p), orientation(Vec3f(0, 0, 1.0)),
              fov(PI / 2.0f), plane_distance(1.0f) {}

    Camera(const Pos3f &p, const Vec3f &o)
            : position(p), orientation(o),
              fov(PI / 2.0f), plane_distance(1.0f) {}

    Camera(const Pos3f &p, const Vec3f &o, const float &f)
            : position(p), orientation(o),
              fov(f), plane_distance(1.0f) {}

    Camera(const Pos3f &p, const Vec3f &o, const float &f, const float &d)
            : position(p), orientation(o),
              fov(f), plane_distance(d) {}
};


