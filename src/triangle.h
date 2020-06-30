#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include "vectors.h"
#include "textures.h"

typedef struct {
    int a;
    int b;
    int c;
    tex2_t a_uv;
    tex2_t b_uv;
    tex2_t c_uv;
    uint32_t color;
} face_t;

typedef struct {
    vec2_t points[3];
    tex2_t tex_coords[3];
    uint32_t color;
    double avg_depth;
} triangle_t;

#endif
