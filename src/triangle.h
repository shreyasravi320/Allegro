#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <iostream>
#include "vectors.h"

typedef struct {
    int a;
    int b;
    int c;
    uint32_t color;
} face_t;

typedef struct {
    vec4_t points[3];
    uint32_t color;
    double avg_depth;
} triangle_t;

#endif
