#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include "vectors.h"
#include "matrix.h"
#include "mesh.h"

using namespace std;

void anim_rot(mesh_t& mesh, double rx, double ry, double rz);
void anim_pos(mesh_t& mesh, double dx, double dy, double dz);
void anim_scale(mesh_t& mesh, double sx, double sy, double sz);

void anim_line(mesh_t& mesh, double xs, double ys, double zs, double xe, double ye, double ze, double frames);

void anim_parabola(mesh_t& mesh, double xs, double max_h, double xe, double zs, double ze, double frames);

void anim_bounce(mesh_t& mesh, double xs, double max_h, double xe, double zs, double ze, int bounces, double frames, double current_frame);

void anim_sin(double& value, double frame, double speed);



#endif
