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

/*
 * The intensity at a given pixel of a sine wave that extends across the field.
 * Angle and offset are in radians, frequency is in cycles per pixel.
 */
float rotated_sine(size_t x, size_t y, float angle, float frequency, float offset) {
    return std::sin((std::cos(angle) * x + std::sin(angle) * y) * frequency + offset);
}

void render_sine(int width, int height, std::vector<Vec3f> &buffer) {
    float sine_freq = 1.0f / 30.0f;

    for (size_t j = 0; j < height; j++) {
        for (size_t i = 0; i < width; i++) {
            float wave_a = rotated_sine(i, j, PI / 4.0f, sine_freq / 2, 0);
            float wave_b = rotated_sine(i, j, PI, sine_freq, 0);

            float b = wave_a + wave_b;
            float r = wave_b + -(wave_a / 2);
            float g = wave_b + -(wave_a / 2);
            buffer[i + j * width] = Vec3f(r, g, b);
        }
    }
}

/*
 * Render an image of the given dimensions into the provided framebuffer.
 */
void render(const size_t &width, const size_t &height, std::vector<Vec3f> &buffer) {
    // Scene setup
    const Pos3f cam_pos(0, 0, 0);
    const Vec3f cam_ori(0, 0, 1.0f);
    const float cam_fov = PI / 2.0f;
    Camera camera(cam_pos, cam_ori, cam_fov);
    Vec3f background_colour(0.0, 0.0, 0.0);
    Vec3f ambient_colour(0.05, 0.03, 0.04);
    Scene scene(camera, Material(background_colour), ambient_colour);

    // Colours
    Vec3f white(1.0, 1.0, 1.0);
    Vec3f red(1.0, 0.0, 0.0);
    Vec3f green(0.0, 1.0, 0.0);
    Vec3f blue(0.0, 0.0, 1.0);

    // Objects to render
    scene.add_sphere(Pos3f(-5, 5, 15), 3.0, Material(Vec3f(0.8, 0.4, 0.8)));
    scene.add_sphere(Pos3f(-2, 3, 20), 4.0, Material(Vec3f(0.2, 0.6, 0.3)));
    scene.add_sphere(Pos3f(10, -5, 16), 3.0, Material(Vec3f(0.4, 0.4, 0.3)));

    scene.add_sphere(Pos3f(10, 5, 30), 3.0, Material(white));
    scene.add_sphere(Pos3f(12, 5, 30), 3.0, Material(white));
    scene.add_sphere(Pos3f(10, 10, 30), 3.0, Material(white));
    scene.add_sphere(Pos3f(12, 10, 30), 3.0, Material(white));

    for (int j = 0; j < 3; j++) {
        for (int i = 0; i < 3; i++) {
            scene.add_sphere(Pos3f((float) ((i - 1) * 5.0), (float) ((j - 1) * 5.0), 10), 1.0, Material(white));
        }
    }

    // Lights
    scene.add_light(Pos3f(2.5, 0, 10), red + blue + green / 4, 2.0);
    scene.add_light(Pos3f(0, 50, 15), red, 1500.0);
    scene.add_light(Pos3f(0, -50, 5), green, 1500.0);
    scene.add_light(Pos3f(50, 0, 15), blue, 1500.0);

    scene.render(width, height, buffer);

}

float clamp_unit(float f) {
    return std::max(0.f, std::min(1.f, f));
}

/*
 * Output the contents of the provided buffer into a ppm file at the specified path.
 * Each colour channel is clamped independently to the range [0.0, 1.0].
 */
void output_ppm(const int &width, const int &height, const std::vector<Vec3f> &buffer, const char *out_path) {
    std::ofstream ofs;
    ofs.open(out_path);
    ofs << "P6\n" << width << " " << height << "\n255\n";

    for (size_t p = 0; p < width * height; p++) {
        ofs << (char) (255 * clamp_unit(buffer[p][0]));
        ofs << (char) (255 * clamp_unit(buffer[p][1]));
        ofs << (char) (255 * clamp_unit(buffer[p][2]));
    }

    ofs.close();
}

int main() {
    char out_path[] = "./out.ppm";
    const size_t width = 1000;
    const size_t height = 1000;

    std::vector<Vec3f> buffer(width * height);
    render(width, height, buffer);
    output_ppm(width, height, buffer, out_path);

    return 0;
}
