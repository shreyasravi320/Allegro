#include "vectors.h"

// 2D Vector Functions
float vec2_mag(vec2_t v) {
    return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t v1, vec2_t v2) {
    vec2_t sum = { .x = (v1.x + v2.x), .y = (v1.y + v2.y) };
    return sum;
}

vec2_t vec2_sub(vec2_t v1, vec2_t v2) {
    vec2_t diff = { .x = (v1.x - v2.x), .y = (v1.y - v2.y) };
    return diff;
}

vec2_t vec2_mul(vec2_t v, float c) {
    vec2_t pro = { .x = (v.x * c), .y = (v.y * c) };
    return pro;
}

vec2_t vec2_div(vec2_t v, float c) {
    vec2_t quo = { .x = (v.x / c), .y = (v.y / c) };
    return quo;
}

float vec2_dot(vec2_t v1, vec2_t v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

void vec2_unit(vec2_t* v) {
    float len = sqrt(v->x * v->x + v->y * v->y);
    v->x /= len;
    v->y /= len;
}

// 3D Vector Functions
float vec3_mag(vec3_t v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t v1, vec3_t v2) {
    vec3_t sum = { .x = (v1.x + v2.x), .y = (v1.y + v2.y), .z = (v1.z + v2.z) };
    return sum;
}

vec3_t vec3_sub(vec3_t v1, vec3_t v2) {
    vec3_t diff = { .x = (v1.x - v2.x), .y = (v1.y - v2.y), .z = (v1.z - v2.z) };
    return diff;
}

vec3_t vec3_mul(vec3_t v, float c) {
    vec3_t pro = { .x = (v.x * c), .y = (v.y * c), .z = (v.z * c) };
    return pro;
}

vec3_t vec3_div(vec3_t v, float c) {
    vec3_t quo = { .x = (v.x / c), .y = (v.y / c), .z = (v.z / c) };
    return quo;
}

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

vec3_t vec3_norm(vec3_t v1, vec3_t v2) {
    vec3_t normal = {
        .x = v1.y * v2.z - v1.z * v2.y,
        .y = v1.z * v2.x - v2.z * v1.x,
        .z = v1.x * v2.y - v2.x * v1.y
    };

    return normal;
}

float vec3_dot(vec3_t v1, vec3_t v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

void vec3_unit(vec3_t* v) {
    float len = sqrt(v->x * v->x + v->y * v->y + v->z * v->z);
    v->x /= len;
    v->y /= len;
    v->z /= len;
}
