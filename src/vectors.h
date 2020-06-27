#ifndef VECTORS_H
#define VECTORS_H

#include <iostream>
#include <math.h>

using namespace std;

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

typedef struct {
    float x;
    float y;
    float z;
    float w;
} vec4_t;

// 2D Vector Functions
float vec2_mag(vec2_t v);

vec2_t vec2_add(vec2_t v1, vec2_t v2);

vec2_t vec2_sub(vec2_t v1, vec2_t v2);

vec2_t vec2_mul(vec2_t v, float c);

vec2_t vec2_div(vec2_t v, float c);

// Vector Dot Product formula
// scalar dot_product = v1.x * v2.x + v1.y * v2.y

float vec2_dot(vec2_t v1, vec2_t v2);

// Get Unit Vector
// unit = v/mag(v)

void vec2_unit(vec2_t* v);

// 3D Vector Functions
float vec3_mag(vec3_t v);

vec3_t vec3_add(vec3_t v1, vec3_t v2);

vec3_t vec3_sub(vec3_t v1, vec3_t v2);

vec3_t vec3_mul(vec3_t v, float c);

vec3_t vec3_div(vec3_t v, float c);

// Vector Rotation around Z-axis (Z position doesn't change)
// Change variables to rotate around different axes
// x = r cos(a)     y = r sin(a)
// x' = r cos(a + b)     y' = r sin(a + b)

// By trig sum identity
// x' = r (cos(a)cos(b) - sin(a)sin(b))
// x' = [r cos(a)]cos(b) - [r sin(a)]sin(b)
// x' = x cos(b) - y sin(b)

// y' = r (sin(a)cos(b) + cos(a)sin(b))
// y' = [r sin(a)]cos(b) + [r cos(a)]sin(b)
// y' = y cos(b) + x sin(b)

vec3_t vec3_rotate_x(vec3_t v, float theta);

vec3_t vec3_rotate_y(vec3_t v, float theta);

vec3_t vec3_rotate_z(vec3_t v, float theta);

// Calculating Normal (Cross Product) of 2 Vectors
// N = normal vector3D
// v1 = vector3D 1, v2 = vector3D 2
//
// N.x = v1.y * v2.z - v1.z * v2.y
// N.y = v1.z * v2.x - v2.z * v1.x
// N.z = v1.x * v2.y - v2.x * v1.y

vec3_t vec3_norm(vec3_t v1, vec3_t v2);

// Vector Dot Product formula
// scalar dot_product = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z

float vec3_dot(vec3_t v1, vec3_t v2);

// Get Unit Vector
// unit = v/mag(v)

void vec3_unit(vec3_t* v);

// Vector 4 functions
vec4_t vec4_from_vec3(vec3_t v);

vec3_t vec4_to_vec3(vec4_t v);

#endif
