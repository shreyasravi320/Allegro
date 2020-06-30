#ifndef TEXTURES_H
#define TEXTURES_H

#include <iostream>
#include "vectors.h"

using namespace std;

typedef struct {
    float u;
    float v;
} tex2_t;

extern int tex_width;

extern int tex_height;

extern const uint8_t RED_BRICK_TEXTURE[];

extern uint32_t* mesh_texture;

vec3_t barycentric_weights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);

#endif
