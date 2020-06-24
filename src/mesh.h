#ifndef MESH_H
#define MESH_H

#include "vectors.h"
#include "triangle.h"

// Vertices for a cube
#define N_MESH_VERTICES (8)
extern vec3_t mesh_vertices[N_MESH_VERTICES];

// 6 cube faces, 2 triangles per face
#define N_MESH_FACES (12)
extern face_t mesh_faces[N_MESH_FACES];

#endif
