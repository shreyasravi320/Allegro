#include <iostream>
#include <vector>
#include "vectors.h"
#include "display.h"
#include "mesh.h"
#include "color.h"

using namespace std;

// Mode control
// 1 = edge, 2 = edge + vertex, 3 = face + vertex , 4 = face + edge, 5 = face + vertex + edge
// c = toggle backface culling

int control = 0;
int cull = 0;

vector<triangle_t> triangles_to_render;  // Need dynamically allocated memory since each mesh has different size

bool is_running = false;

// Field of  factor to scale up points
float fov_factor = 1024;

// Camera origin
vec3_t camera_position = { .x = 0, .y = 0, .z = 0 };

// Frame Rate Setup
int prev_frame_time;
int time_to_wait;

// Merge sort algorithm for depth sorting

void merge(vector<triangle_t>& parent, vector<triangle_t>& left, int left_size, vector<triangle_t>& right, int right_size) {
    int parent_idx = 0, left_idx = 0, right_idx = 0;

    while(left_idx < left_size && right_idx < right_size) {
        if(left[left_idx].avg_depth <= right[right_idx].avg_depth) {
            parent[parent_idx++] = left[left_idx++];
        }
        else {
            parent[parent_idx++] = right[right_idx++];
        }
    }
    while(left_idx < left_size) {
        parent[parent_idx++] = left[left_idx++];
    }
    while(right_idx < right_size) {
        parent[parent_idx++] = right[right_idx++];
    }
}

void merge_sort(vector<triangle_t>& v, int size) {

    if(size < 2) {
        return;
    }

    int left_size = size / 2;
    int right_size = size - left_size;

    vector<triangle_t> left;
    vector<triangle_t> right;
    for(int i = 0; i < left_size; ++i) {
        left.push_back(v[i]);
    }
    for(int i = 0; i < right_size; ++i) {
        right.push_back(v[i + left_size]);
    }

    merge_sort(left, left_size);
    merge_sort(right, right_size);

    merge(v, left, left_size, right, right_size);

    left.clear();
    left.shrink_to_fit();
    right.clear();
    right.shrink_to_fit();
}

void setup() {

    // Allocate 32 bits of memory for color for each pixel on the screen
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * win_width * win_height);

    // Pass the color buffer data to the color buffer texture
    color_buffer_texture = SDL_CreateTexture(
        renderer,   // Pointer to renderer
        SDL_PIXELFORMAT_ARGB8888,   // Pixel format (argb, 8 bits each)
        SDL_TEXTUREACCESS_STREAMING,    // Access type - we are updating every frame so use streaming
        win_width,  // Width to apply over
        win_height  // Height to apply over
    );

    // Loads the cube values in the mesh data structure
    // load_cube_mesh_data();

    // Load the obj mesh data
    load_obj_mesh_data("./models/suzanne.obj");
}

void process_input() {

    SDL_Event event;

    SDL_PollEvent(&event);

    switch (event.type) {

        case SDL_QUIT:

            is_running = false;

            break;

        case SDL_KEYDOWN:

            if(event.key.keysym.sym == SDLK_q) {    // Quit on Q key
                is_running = false;
            }

            if(event.key.keysym.sym == SDLK_1) {
                control = 1;
            }

            if(event.key.keysym.sym == SDLK_2) {
                control = 2;
            }

            if(event.key.keysym.sym == SDLK_3) {
                control = 3;
            }

            if(event.key.keysym.sym == SDLK_4) {
                control = 4;
            }

            if(event.key.keysym.sym == SDLK_5) {
                control = 5;
            }

            if(event.key.keysym.sym == SDLK_c) {
                cull++;
            }

            if(event.key.keysym.sym == SDLK_0) {
                control = 0;
            }

            break;
    }
}

// Orthographic
vec2_t project_ortho(vec3_t point) {
    // Convert 3D point to 2D on x and y axis
    vec2_t projected_point = {
        .x = (fov_factor * point.x),    // Multiply by field ov  factor to scale the point up
        .y = (fov_factor * point.y)
    };

    return projected_point;
}

vec2_t project_persp(vec3_t point) {

    // Convert 3D point to 2D on x and y axis
    vec2_t projected_point = {
        .x = (fov_factor * point.x / point.z),  // Similar triangles formula to scale down point in perspective
        .y = (fov_factor * point.y / point.z)
    };

    return projected_point;
}

vec2_t project(vec3_t point) {
    return project_persp(point);
}

void update() {

    time_to_wait = FRAME_TARGET_LENGTH - (SDL_GetTicks() - prev_frame_time);
    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_LENGTH) {    // Delay execution until the time passed is the Frame Target Length
        SDL_Delay(time_to_wait);
    }

    prev_frame_time = SDL_GetTicks();

    // Initialize triangle array
    triangles_to_render.clear();

    // Transformations to mesh
    mesh.rotation.x += 0.0075;
    mesh.rotation.y += 0.0075;
    mesh.rotation.z += 0.0075;

    // Loop through vertices and faces

    for(int i = 0; i < mesh.faces.size(); i++) {

        face_t current_face = mesh.faces[i];
        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[current_face.a - 1];   // our current face_t a index starts at 1, so we need to subtract 1 to compensate
        face_vertices[1] = mesh.vertices[current_face.b - 1];
        face_vertices[2] = mesh.vertices[current_face.c - 1];


        // Loop through all vertices of the current face, apply any transformations, and project the points

        vec3_t transformed_vertices[3];

        for(int j = 0; j < 3; j++) {

            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Translate vertex towards from camera in Z-axis if in Persp
            transformed_vertex.z -= 5;
            transformed_vertices[j] = transformed_vertex;
        }

        // Back-face Culling Algorithm
        // Calculate normal
        // Calculate dot product between normal and camera ray to triangle
        // if dot product < 0, cull that face

        // A---B
        // |  /
        // | /
        // C
        if(cull % 2 == 0) {
            vec3_t a = transformed_vertices[0];
            vec3_t b = transformed_vertices[1];
            vec3_t c = transformed_vertices[2];

            // Vector subtraction to get b-a and c-a -> use those to calculate normal
            vec3_t ab = vec3_sub(a, b);
            vec3_t ac = vec3_sub(a, c);

            // Convert to unit vector - only direction matters
            vec3_unit(&ab);
            vec3_unit(&ac);

            // Calculate normal based on orientation - we are using right handed orientation (z is positive towards us)
            vec3_t norm = vec3_norm(ab, ac);

            // Convert to unit vector - only direction matters
            vec3_unit(&norm);

            // Check if dotproduct between normal and camera ray to vector a is < 0
            vec3_t camera_ray = vec3_sub(camera_position, a);
            if(vec3_dot(norm, camera_ray) < 0) {
                continue;   // Bypass the rest of the for loop and restart
            }
        }

        vec2_t projected_points[3];
        for(int j = 0; j < 3; j++) {

            // Project current point
            projected_points[j] = project(transformed_vertices[j]);

            // Translate projected point to the middle of the screen
            projected_points[j].x += win_width/2;
            projected_points[j].y += win_height/2;
        }

        // Calculate average depth of each face so we can sort and render back to front
        float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

        triangle_t projected_triangle = {
            .points = {
                projected_points[0],
                projected_points[1],
                projected_points[2]
            },
            .color = current_face.color,
            .avg_depth = avg_depth
        };

        // Save the projected triangle in the array of triangles to render
        // triangles_to_render[i] = projected_triangle;
        triangles_to_render.push_back(projected_triangle);
    }

    // Sort triangles_to_render by average depth least to greatest
    merge_sort(triangles_to_render, triangles_to_render.size());
}

void render() {

    draw_grid(60, 0xFF444444);  // Draw a gray grid

    // Loop through array and render all projected triangles
    int size = triangles_to_render.size();

    for(int i = 0; i < size; i++) {

        triangle_t triangle = triangles_to_render[i];

        if(control == 0 || control == 3 || control == 4 || control == 5) {
            // Draw filled triangle from vertices
            draw_filled_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                triangle.color
            );
        }

        if(control == 0 || control == 1 || control == 2 || control == 4 || control == 5) {
            // Draw triangle edges from vertices
            draw_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                black
            );
        }

        if(control == 0 || control == 2 || control == 3 || control == 5) {
            // Draw all vertices on top of edges
            draw_rect(triangle.points[0].x, triangle.points[0].y, 5, 5, red);
            draw_rect(triangle.points[1].x, triangle.points[1].y, 5, 5, red);
            draw_rect(triangle.points[2].x, triangle.points[2].y, 5, 5, red);
        }
    }

    // Free memory used by triangles_to_render array
    triangles_to_render.clear();

    render_color_buffer();  // Render the color buffer texture

    clear_color_buffer(0xFF222222);   // Clear the color buffer to gray (hexa)

    SDL_RenderPresent(renderer);    // Draw on the renderer
}

void free_resources() {

    // Free dynamically allocated memory after program quits
    mesh.vertices.clear();
    mesh.vertices.shrink_to_fit();
    mesh.faces.clear();
    mesh.faces.shrink_to_fit();
    free(color_buffer);
}

int main() {

    // Create an SDL Window
    is_running = init_window();

    setup();

    while(is_running) {
        process_input();
        update();
        render();
    }

    quit();
    free_resources();

    return 0;
}
