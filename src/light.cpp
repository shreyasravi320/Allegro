#include "light.h"

using namespace std;

light_t world_light = {
    .direction = { .x = 0, .y = 0, .z = 1 },
    .intensity = 1.0
};

uint32_t calculate_shading(uint32_t original_color, double shading_factor) {
    uint32_t a = (original_color & 0xFF000000);
    uint32_t r = (original_color & 0x00FF0000) * pow(shading_factor, 1);
    uint32_t g = (original_color & 0x0000FF00) * pow(shading_factor, 1);
    uint32_t b = (original_color & 0x000000FF) * pow(shading_factor, 1);

    return a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
}

// Flat shading: color the triangle face based on the alignment of face normal and light ray
uint32_t shade_flat(vec3_t normal, light_t light, uint32_t color) {
    double shading_amount = vec3_dot(normal, light.direction);
    shading_amount = (shading_amount + 1) / 2 * light.intensity;
    return calculate_shading(color, shading_amount);
}
