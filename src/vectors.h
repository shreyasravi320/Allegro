#ifndef VECTORS_H
#define VECTORS_H

#include <math.h>

typedef struct {
    float x;
    float y;
} vec2_t;

typedef struct {
    float x;
    float y;
    float z;
} vec3_t;

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

#endif
