#include "vectors.h"

vec3_t vec3_rotate_x(vec3_t v, float theta) {
    float x_new = v.x;
    float y_new = v.y * cos(theta) - v.z * sin(theta);
    float z_new = v.y * sin(theta) + v.z * cos(theta);
    vec3_t v_new = { .x = x_new, .y = y_new, .z = z_new };

    return v_new;
}

vec3_t vec3_rotate_y(vec3_t v, float theta) {
    float x_new = v.x * cos(theta) - v.z * sin(theta);
    float y_new = v.y;
    float z_new = v.x * sin(theta) + v.z * cos(theta);
    vec3_t v_new = { .x = x_new, .y = y_new, .z = z_new };

    return v_new;
}

vec3_t vec3_rotate_z(vec3_t v, float theta) {
    float x_new = v.x * cos(theta) - v.y * sin(theta);
    float y_new = v.y * cos(theta) + v.x * sin(theta);
    float z_new = v.z;
    vec3_t v_new = { .x = x_new, .y = y_new, .z = z_new };

    return v_new;
}
