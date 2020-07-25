#ifndef SCENE_H
#define SCENE_H

#include <iostream>
#include <vector>
#include "vectors.h"
#include "display.h"
#include "mesh.h"
#include "color.h"
#include "matrix.h"
#include "light.h"
#include "animation.h"
#include "simulations.h"
#include "helpers.h"

using namespace std;

void setup();
void process_input();
vec2_t project_ortho(vec3_t point);
vec2_t project_persp(vec3_t point);
vec2_t project(vec3_t point);
void update();
void render();
void free_resources();

#endif
