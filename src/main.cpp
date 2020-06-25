#include <iostream>
#include <vector>
#include "vectors.h"
#include "display.h"
#include "mesh.h"

using namespace std;

vector<triangle_t> triangles_to_render;  // Need dynamically allocated memory since each mesh has different size

bool is_running = false;

// Field of view factor to scale up points
float fov_factor = 1024;

// Camera origin
vec3_t camera_position = { .x = 0, .y = 0, .z = -5 };

// View control
int view = 0;

// Frame Rate Setup
int prev_frame_time;
int time_to_wait;

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
    load_obj_mesh_data("./models/cube.obj");
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
                mesh.vertices.clear();
                mesh.faces.clear();
                load_obj_mesh_data("./models/sphere.obj");
            }

            if(event.key.keysym.sym == SDLK_2) {
                mesh.vertices.clear();
                mesh.faces.clear();
                load_obj_mesh_data("./models/torus.obj");
            }

            if(event.key.keysym.sym == SDLK_3) {
                mesh.vertices.clear();
                mesh.faces.clear();
                load_obj_mesh_data("./models/suzanne.obj");
            }

            break;
    }
}

// Orthographic View
vec2_t project_ortho(vec3_t point) {
    // Convert 3D point to 2D on x and y axis
    vec2_t projected_point = {
        .x = (fov_factor * point.x),    // Multiply by field ov view factor to scale the point up
        .y = (fov_factor * point.z)
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

vec2_t project(int view, vec3_t point) {
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
    // mesh.rotation.y += 0.0075;
    // mesh.rotation.z += 0.0075;

    // Loop through vertices and faces

    for(int i = 0; i < mesh.faces.size(); i++) {

        face_t current_face = mesh.faces[i];
        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[current_face.a - 1];   // our current face_t a index starts at 1, so we need to subtract 1 to compensate
        face_vertices[1] = mesh.vertices[current_face.b - 1];
        face_vertices[2] = mesh.vertices[current_face.c - 1];

        triangle_t projected_triangle;

        // Loop through all vertices of the current face, apply any transformations, and project the points
        for(int j = 0; j < 3; j++) {

            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex = vec3_rotate_x(transformed_vertex, mesh.rotation.x);
            transformed_vertex = vec3_rotate_y(transformed_vertex, mesh.rotation.y);
            transformed_vertex = vec3_rotate_z(transformed_vertex, mesh.rotation.z);

            // Translate vertex away from camera in Z-axis if in Persp
            transformed_vertex.z += camera_position.z;

            // Project current point
            vec2_t projected_point = project(view, transformed_vertex);

            // Translate projected point to the middle of the screen
            projected_point.x += win_width/2;
            projected_point.y += win_height/2;

            // Add the projected point to the projected triangle
            projected_triangle.points[j] = projected_point;
        }

        // Save the projected triangle in the array of triangles to render
        // triangles_to_render[i] = projected_triangle;
        triangles_to_render.push_back(projected_triangle);
    }
}

void render() {

    draw_grid(60, 0xFF606060);  // Draw a gray grid

    // Loop through array and render all projected triangles
    int size = triangles_to_render.size();

    for(int i = 0; i < size; i++) {

        triangle_t triangle = triangles_to_render[i];

        // Draw unfilled triangle outline from vertices
        draw_triangle(
            triangle.points[0].x,
            triangle.points[0].y,
            triangle.points[1].x,
            triangle.points[1].y,
            triangle.points[2].x,
            triangle.points[2].y,
            0xFFFFFF00
        );

        // Draw all vertices on top of edges
        draw_rect(triangle.points[0].x, triangle.points[0].y, 5, 5, 0xFFFF0000);
        draw_rect(triangle.points[1].x, triangle.points[1].y, 5, 5, 0xFFFF0000);
        draw_rect(triangle.points[2].x, triangle.points[2].y, 5, 5, 0xFFFF0000);
    }

    // Free memory used by triangles_to_render array
    triangles_to_render.clear();

    render_color_buffer();  // Render the color buffer texture

    clear_color_buffer(0xFF000000);   // Clear the color buffer to gray (hexa)

    SDL_RenderPresent(renderer);    // Draw on the renderer
}

void free_resources() {

    // Free dynamically allocated memory after program quits
    mesh.vertices.clear();
    mesh.faces.clear();
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
