#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <math.h>
#include "vectors.h"

using namespace std;

typedef struct {
    float mat[4][4];
} mat4_t;

mat4_t mat4_identity();

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);

mat4_t mat4_mul_mat(mat4_t a, mat4_t b);

mat4_t mat4_scale(float sx, float sy, float sz);

mat4_t mat4_translate(float dx, float dy, float dz);

mat4_t mat4_rotate_z(float theta);

mat4_t mat4_rotate_x(float theta);

mat4_t mat4_rotate_y(float theta);

mat4_t mat4_project(float aspect, float fov, float z_near, float z_far);

vec4_t mat4_persp_divide(mat4_t m_proj, vec4_t v);

#endif
