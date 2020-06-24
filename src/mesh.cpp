#include "mesh.h"

using namespace std;

mesh_t mesh = {
    .rotation = { .x = 0, .y = 0, .z = 0 }
};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    { .x = -1, .y = -1, .z = -1 }, // 1
    { .x = -1, .y =  1, .z = -1 }, // 2
    { .x =  1, .y =  1, .z = -1 }, // 3
    { .x =  1, .y = -1, .z = -1 }, // 4
    { .x =  1, .y =  1, .z =  1 }, // 5
    { .x =  1, .y = -1, .z =  1 }, // 6
    { .x = -1, .y =  1, .z =  1 }, // 7
    { .x = -1, .y = -1, .z =  1 }  // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    { .a = 1, .b = 2, .c = 3 },
    { .a = 1, .b = 3, .c = 4 },
    // right
    { .a = 4, .b = 3, .c = 5 },
    { .a = 4, .b = 5, .c = 6 },
    // back
    { .a = 6, .b = 5, .c = 7 },
    { .a = 6, .b = 7, .c = 8 },
    // left
    { .a = 8, .b = 7, .c = 2 },
    { .a = 8, .b = 2, .c = 1 },
    // top
    { .a = 2, .b = 7, .c = 5 },
    { .a = 2, .b = 5, .c = 3 },
    // bottom
    { .a = 6, .b = 8, .c = 1 },
    { .a = 6, .b = 1, .c = 4 }
};

void load_cube_mesh_data() {

    // Loop through all cube vertices and push
    for(int i = 0; i < N_CUBE_VERTICES; i++) {
        mesh.vertices.push_back(cube_vertices[i]);
    }

    // Loop through all cube faces and push
    for(int i = 0; i < N_CUBE_FACES; i++) {
        mesh.faces.push_back(cube_faces[i]);
    }
}

void load_obj_mesh_data(string file) {

    // Read contents of obj file
    ifstream in_file;
    in_file.open(file);

    // Check if file was able to opened
    if (!in_file) {
        cout << "Unable to open file " << file << endl;
        exit(1);   // call system to stop
    }

    char* line;

    // Read vertices
    while(!in_file.eof()) {

        if(line[0] == 'v' && line[1] == ' ') {
            vec3_t vertex;
            // Read the format of the line and store values in vertex data
            // sscanf(line to read, format, where to store values - must be loaded by reference (&))
            sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
            mesh.vertices.push_back(vertex);
        }
    }

    // Read faces
    while(!in_file.eof()) {
        if(line[0] == 'f' && line[1] == ' ') {

            int vertex_indices[3];
            int texture_indices[3];
            int normal_indices[3];
            // Read the format of the line and store values in face data
            // sscanf(line to read, format, where to store values - must be loaded by reference (&))
            sscanf(
                line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                &vertex_indices[2], &texture_indices[2], &normal_indices[2]
            );

            // Load the vertex indices into the face data structure
            face_t face = {
                .a = vertex_indices[0],
                .b = vertex_indices[1],
                .c = vertex_indices[2]
            };

            mesh.faces.push_back(face);
        }
    }
}
