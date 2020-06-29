#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include "vectors.h"
#include <math.h>

using namespace std;

typedef struct {
    vec3_t direction;
    double intensity;
} light_t;

extern light_t world_light;

uint32_t calculate_shading(uint32_t original_color, double shading_factor);

uint32_t shade_flat(vec3_t normal, light_t light, uint32_t color);

#endif
