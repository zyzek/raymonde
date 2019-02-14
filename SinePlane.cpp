#include "SinePlane.hpp"

Ray3f sine_normal(Pos3f position) { // z ordinate is ignored
    const float x_frequency = 1.0;
    const float y_frequency = 1.0;
    const float x_offset = 0.0;
    const float y_offset = 0.0;
    const float z_offset = 0.0;
    const float magnitude = 1.0;

    const float x = position.x * x_frequency + x_offset;
    const float y = position.y * y_frequency + y_offset;

    const float x_height = std::sin(x);
    const float y_height = std::sin(y);
    const float height = magnitude * (x_height + y_height) + z_offset;

    const Pos3f value(position.x, position.y, height);

    const float x_slope = -std::cos(x);
    const float y_slope = -std::cos(y);

    const Vec3f normal(x_slope, y_slope, 1.0);

    return Ray3f(value, normal);
}

bool SinePlane::raycast(const Ray3f &ray, Ray3f &normal) const {
    // 1. approximate as a square wave.
    // 2.
    // (x, y) -> |x,y|
    // z = sin(fx + o) + sin(gy + p)
    // z = ax + by + c
    // Newton's method:
    //const uint iterations = 10;
    //Vec2f guess = ray.position;

    //Vec3f p = (ray.position + ray.direction).vector();
    //p = p / p.z;
    // f(x, y) = sin(fx + o) + sin(gy + p) - ax - by - c
    // (x) f'(x, y) = f * cos(fx + o) - a
    // (y) f'(x, y) = g * cos(gx + p) - b
    // N(x) = x - f(x,y) / f'(x,y)

    const Vec2f xy_offset = offset.xy();

    // 1. Find x-y angle of the line, we're colliding along this slice.
    //    Then y = slope * x;
    //    We also have an expression for z in terms of x.
    const Vec2f xy = ray.direction.xy().unit();
    const Vec2f xz = ray.direction.xz().unit();
    const float xy_slope = xy.y/xy.x;
    const float xz_slope = xz.y/xz.x;

    // 2. Underlying functional form along this slice is then:
    //     z = xy.x * sin(f*x + o) + xy.y * sin(g*y + p)
    //       = xy.x * sin(f*x + o) + xy.y * sin(g*xy_slope*x + p)
    //    We seek the root of
    //     f(x) = xy.x * sin(f*x + o) + xy.y * sin(g*xy_slope*x + p) - xz_slope*x
    //    Which implies the derivative:
    //     f'(x) = xy.x * f*cos(f*x + o) + xy.y * g*slope*cos(g*slope*x + p) - xz_slope

    const Vec2f position = ray.position.xy().vector();

    const Vec2f args = hadamard(position, frequency) + xy_offset;

    const Vec2f fxv = hadamard(xy, Vec2f(std::sin(args.x), std::sin(args.y)));
    const Vec2f dfxv = hadamard(Vec2f(frequency.x, frequency.y * xy_slope), fxv);
    const float fx = fxv.x + fxv.y - xz_slope*position.x;
    const float dfx = dfxv.x + dfxv.y - xz_slope;
}

Vec3f SinePlane::surface_colour(const Ray3f &incident_ray, const Ray3f &collision_normal, const Scene &scene) const;

Ray3f SinePlane::displace_normal_outward(const Ray3f &normal) const;

