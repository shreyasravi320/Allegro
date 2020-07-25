#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "vectors.h"
#include "triangle.h"
#include "color.h"

using namespace std;

// Vertices for a cube
#define N_CUBE_VERTICES (8)
extern vec3_t cube_vertices[N_CUBE_VERTICES];

// 6 cube faces, 2 triangles per face
#define N_CUBE_FACES (12)
extern face_t cube_faces[N_CUBE_FACES];

// Struct for generic mesh
typedef struct {
    vector<vec3_t> vertices;    // Dynamic array of vertices
    vector<face_t> faces;       // Dynamic array of faces
    vec3_t rot;                 // Rotation vector
    vec3_t scale;               // Scale vector
    vec3_t pos;                 // Position vector
} mesh_t;

// Global mesh variable
extern mesh_t mesh;

extern vec3_t default_pos;
extern vec3_t default_scale;
extern vec3_t default_rot;

void load_cube_mesh_data();

mesh_t load_obj_mesh_data(mesh_t mesh, string file, uint32_t color);

#endif
