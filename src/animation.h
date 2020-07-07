#ifndef ANIMATION_H
#define ANIMATION_H

#include <iostream>
#include <vector>
#include "vectors.h"
#include "matrix.h"
#include "mesh.h"
#include "display.h"
#include "helpers.h"

using namespace std;

void anim_rot(mesh_t& mesh, double rx, double ry, double rz);
void anim_pos(mesh_t& mesh, double dx, double dy, double dz);
void anim_scale(mesh_t& mesh, double sx, double sy, double sz);

void anim_line(mesh_t& mesh, double xs, double ys, double zs, double xe, double ye, double ze, double frames);

void anim_parabola(mesh_t& mesh, double xs, double max_h, double xe, double zs, double ze, double frames);

void anim_bounce(mesh_t& mesh, double xs, double max_h, double xe, double zs, double ze, int bounces, double frames, double current_frame);


// Forward and Inverse Kinematics

class joint_t {
public:
    vec3_t pos1;
    double len;
    double theta;
    double self_theta;
    vec3_t pos2;
    joint_t* parent = nullptr;
};

joint_t fk_create_joint(double x, double y, double len, double theta);

joint_t fk_create_joint(joint_t* parent, double len, double theta);

void follow(joint_t& joint, double dx, double dy);

joint_t ik_create_joint(double x, double y, double len, double theta);

joint_t ik_create_joint(joint_t* parent, double len, double theta);

void ik_set_base(joint_t& joint, vec3_t pos);

void fk_joint_render(joint_t& joint, uint32_t color);

void ik_joint_render(joint_t& joint, uint32_t color);

#endif
