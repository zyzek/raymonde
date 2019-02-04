#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>

#include "constants.hpp"
#include "Vector.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"
#include "Scene.hpp"


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
 * Render an image of the given dimensions to a file at the given path.
 */
void render(const size_t &width, const size_t &height, const char *out_path) {
    // Camera parameters
    const Vec3f cam_pos = Vec3f(0, 0, 0);
    const Vec3f cam_ori = Vec3f(0, 0, 1.0f);
    const float cam_fov = PI / 2.0f;
    const float plane_dist = 1.0f;
    Camera camera(cam_pos, cam_ori, cam_fov, plane_dist);

    Vec3f background_colour = Vec3f(0.2, 0.7, 0.8);

    Scene scene(camera, Material(background_colour));

    // Object to render
    scene.add_sphere(Vec3f(10, -5, 16), 3.0, Material(Vec3f(0.4, 0.4, 0.3)));
    scene.add_sphere(Vec3f(-5, 5, 13), 4.0, Material(Vec3f(0.8, 0.4, 0.8)));
    scene.add_sphere(Vec3f(-2, 3, 20), 5.0, Material(Vec3f(0.2, 0.6, 0.3)));

    std::vector<Vec3f> framebuffer(width * height);
    scene.render(width, height, framebuffer);

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


int main() {
    const size_t width = 1024;
    const size_t height = 786;
    char out_path[] = "./out.ppm";
    render(width, height, out_path);
    return 0;
}
