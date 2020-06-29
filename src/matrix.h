#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <math.h>
#include "vectors.h"

using namespace std;

typedef struct {
    double mat[4][4];
} mat4_t;

mat4_t mat4_identity();

vec4_t mat4_mul_vec4(mat4_t m, vec4_t v);

mat4_t mat4_mul_mat(mat4_t a, mat4_t b);

mat4_t mat4_scale(double sx, double sy, double sz);

mat4_t mat4_translate(double dx, double dy, double dz);

mat4_t mat4_rotate_z(double theta);

mat4_t mat4_rotate_x(double theta);

mat4_t mat4_rotate_y(double theta);

mat4_t mat4_project(double aspect, double fov, double z_near, double z_far);

vec4_t mat4_persp_divide(mat4_t m_proj, vec4_t v);

#endif
