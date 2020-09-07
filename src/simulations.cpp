#include "simulations.h"

using namespace std;

const double PI = 3.141592653;

double fixed_height = default_pos.y;
double depth = 0.05;
double gravity = 3;

// Liquid wave animation using Simple Harmonic Motion
double fix_y = default_pos.y;

wave_t create_wave() {
    vec3_t wave_pos = { .x = 0, .y = -3, .z = 40 };
    mesh_t wave_mesh = { .rot = default_rot, .scale = default_scale, .pos = wave_pos };
    wave_t wave = { .mesh = load_obj_mesh_data(wave_mesh, "./models/plane.obj", blue) };
    return wave;
}

void simulate_waves(mesh_t& mesh, double frames, double current_frame) {
    double period = frames;
    // double wavelength = period * sqrt(depth * gravity);

    for(int i = 0; i < mesh.vertices.size(); i++) {
        double wavelength = gravity / (2 * PI) * period * period * tanh(2 * PI / wavelength * depth);
        double theta = 2 * PI / wavelength * mesh.vertices[i].x - 2 * PI / period * current_frame;
        double amplitude = 0.075;
        mesh.vertices[i].y = amplitude * sin(theta);
    }
}

void splash(wave_t wave) {

}
