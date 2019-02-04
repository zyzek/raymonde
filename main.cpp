#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "vector.hpp"
#include "sphere.hpp"

#define PI 3.1415926535f

float clamp_unit(float f) {
    return std::max(0.f, std::min(1.f, f));
}

/*
 * The intensity at a given pixel of a sine wave that extends across the field.
 * Angle and offset are in radians, frequency is in cycles per pixel.
 */
float rotated_sine(size_t x, size_t y, float angle, float frequency, float offset) {
    return std::sin((std::cos(angle) * x + std::sin(angle) * y) * frequency + offset);
}

void render_sine(int width, int height, const char *out_path) {
    std::vector<Vec3f> framebuffer(width * height);
    float sine_freq = 1.0f / 30.0f;

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            float wave_a = rotated_sine(i, j, PI / 4.0, sine_freq / 2, 0);
            float wave_b = rotated_sine(i, j, PI, sine_freq, 0);

            float b = wave_a + wave_b;
            float r = wave_b + -(wave_a / 2);
            float g = wave_b + -(wave_a / 2);
            framebuffer[i + j * width] = Vec3f(r, g, b);
        }
    }

    std::ofstream ofs;
    ofs.open(out_path);
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (size_t p = 0; p < width * height; p++) {
        ofs << (char) (255 * clamp_unit(framebuffer[p][0]));
        ofs << (char) (255 * clamp_unit(framebuffer[p][1]));
        ofs << (char) (255 * clamp_unit(framebuffer[p][2]));
    }

    ofs.close();
}

/*
 * Cast a ray from an origin, towards a sphere, returning the colour of the collision point.
 */
Vec3f raycast(const Vec3f &origin, const Vec3f &direction, const Sphere &sphere) {
    Vec3f background_colour = Vec3f(0.2, 0.7, 0.8);
    float sphere_dist = std::numeric_limits<float>::max();
    if (!sphere.ray_intersects(origin, direction, sphere_dist)) {
        return background_colour;
    }
    return sphere.material.diffuse_colour;
}

/*
 * Render an image of the given dimensions to a file at the given path.
 */
void render(size_t width, size_t height, const char *outFilePath) {
    std::vector<Vec3f> framebuffer(width * height);

    // Object to render
    const Vec3f position(10, -5, 16);
    const float radius(3.0);
    const Vec3f colour(0.4, 0.4, 0.3);
    const Sphere sphere(position, radius, Material(colour));

    // Camera parameters
    const Vec3f camPos = Vec3f(0, 0, 0);
    const float planeDist = 1.0f;
    const float xFov = PI / 2.0f;
    const float xFovTan = std::tan(xFov / 2.0f);
    const float yFovTan = xFovTan * (float) height / (float) width;
    const float xComp = 2.0f * planeDist * xFovTan / ((float) width - 1);
    const float yComp = -2.0f * planeDist * yFovTan / ((float) height - 1);

#pragma omp parallel for
    for (ssize_t j = 0; j < height; j++) {
        const float y = (j - height / 2.0f) * yComp;
        for (ssize_t i = 0; i < width; i++) {
            const float x = (i - width / 2.0f) * xComp;
            Vec3f ray = Vec3f(x, y, planeDist);
            framebuffer[i + j * width] = raycast(camPos, ray, sphere);
        }
    }

    std::ofstream ofs;
    ofs.open(outFilePath);
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (size_t p = 0; p < width * height; p++) {
        ofs << (char) (255 * clamp_unit(framebuffer[p][0]));
        ofs << (char) (255 * clamp_unit(framebuffer[p][1]));
        ofs << (char) (255 * clamp_unit(framebuffer[p][2]));
    }

    ofs.close();
}


int main() {
    const size_t width = 1024;
    const size_t height = 786;
    char outFilePath[] = "./out.ppm";
    render(width, height, outFilePath);
    return 0;
}
