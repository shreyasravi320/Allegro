#ifndef SIMULATIONS_H
#define SIMULATIONS_H

#include "mesh.h"
#include <iostream>
#include <vector>

using namespace std;

typedef struct {
    mesh_t mesh;
    double fix_y;
    double speed;
    double k;
    double df;
} wave_t;

wave_t create_wave();

void simulate_waves(mesh_t& mesh, double frames, double current_frame);

#endif
