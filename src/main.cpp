#include <iostream>
#include <string>
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
#include <unordered_map>
// #include "scene.h"

using namespace std;

// const int joint_size = 60;
// joint_t joints[joint_size];
// int followX, followY;
// vec3_t fixed_base;

wave_t wave;
mesh_t mesh = {
    .rot = default_rot,
    .scale = default_scale,
    .pos = default_pos
};

mat4_t proj_matrix;
// Mode control
// 1 = vertices, 2 = edge, 3 = face, 4 = vertex + edge, 5 = face + vertex, 6 = edge + face, 7 = face + vertex + edge
// 8 = textures, 9 = textures + edges, 0 = vertex + edge + textures
// c = toggle backface culling

int control = 0;
int cull = 0;
double current_frame = 0;

vector<triangle_t> triangles_to_render;  // Need dynamically allocated memory since each mesh has different size

bool is_running = false;

// Field of  factor to scale up points
double fov_factor = 1024;

// Camera origin
vec3_t camera_position = { .x = 0, .y = 0, .z = 0 };

// Frame Rate Setup
int prev_frame_time;
int time_to_wait;

void setup(string model, string color, string size) {
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

    // Initialize the projection matrix
    double aspect = (double) win_height / (double) win_width;
    double fov = PI / 6;  // 90 degrees -> radians
    double z_near = 0.1;   // random value
    double z_far = 100.0; // also random value
    proj_matrix = mat4_project(aspect, fov, z_near, z_far);

    // Loads the cube values in the mesh data structure
    // load_cube_mesh_data();

    // Load the obj mesh data
    uint32_t col = white;
    if(color.size() != 0) {
        col = string_to_color(color);
    }

    string mesh_name = "./models/" + model + ".obj";
    mesh = load_obj_mesh_data(mesh, mesh_name, col);

    double sc = 1;
    if(size == "big") {
        sc = 2;
    }
    mesh.scale = vec3_mul(default_scale, sc);

    // wave = create_wave();
    // mesh = wave.mesh;

    // Initialize fixed base for inverse/forward kinematics
    // fixed_base = { .x = (double) win_width / 2, .y = (double) win_height - 50, .z = 0 };
    //
    // // Create n number of joints for forward/inverse kinematics
    // joints[0] = ik_create_joint(win_width / 2, win_height / 2, 10, PI / 2);
    //
    // for(int i = 1; i < joint_size - 1; i++) {
    //     joints[i] = ik_create_joint(&joints[i - 1], 10, PI / 2);
    // }
    // joints[joint_size - 1] = ik_create_joint(&joints[joint_size - 2], 10, 0);

    // joints[1] = ik_create_joint(&joints[0], 75, 0);
    // joints[2] = ik_create_joint(&joints[1], 75, 0);
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

            if(event.key.keysym.sym == SDLK_6) {
                control = 6;
            }

            if(event.key.keysym.sym == SDLK_7) {
                control = 7;
            }

            if(event.key.keysym.sym == SDLK_8) {
                control = 8;
            }

            if(event.key.keysym.sym == SDLK_9) {
                control = 9;
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
        .x = (fov_factor * point.x),    // Multiply by field of factor to scale the point up
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

    // SDL_GetMouseState(&followX, &followY);

    time_to_wait = FRAME_TARGET_LENGTH - (SDL_GetTicks() - prev_frame_time);
    if(time_to_wait > 0 && time_to_wait <= FRAME_TARGET_LENGTH) {    // Delay execution until the time passed is the Frame Target Length
        SDL_Delay(time_to_wait);
    }

    prev_frame_time = SDL_GetTicks();

    // Initialize triangle array
    triangles_to_render.clear();

    // Perform Animations
    // mesh.scale.x = 2;
    // mesh.scale.y = 2;
    // mesh.scale.z = 2;
    //
    // anim_bounce(mesh, default_pos.x, 1.5, -10, default_pos.z, 22, 5, 30, ++current_frame);

    // simulate_waves(wave, 30, current_frame++);
    // mesh = wave.mesh;
    // mesh.pos.z += 0.1;
    mesh.rot.y += 0.01;
    // mesh.rot.z += 0.01;

    // follow(joints[joint_size - 1], followX, followY);
    // for(int i = joint_size - 2; i >= 1; i--) {
    //     follow(joints[i], joints[i + 1].pos1.x, joints[i + 1].pos1.y);
    // }
    // follow(joints[0], joints[1].pos1.x, joints[1].pos1.y);
    //
    // for(int i = i; i < joint_size; i++) {
    //     ik_set_base(joints[i], joints[i - 1].pos2);
    // }
    // ik_set_base(joints[0], fixed_base);
    //
    // ik_joint_render(joints[0], red);
    // for(int i = 1; i < joint_size - 1; i++) {
    //     ik_joint_render(joints[i], yellow);
    // }
    // ik_joint_render(joints[joint_size - 1], green);

    mat4_t scale_matrix = mat4_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
    mat4_t translate_matrix = mat4_translate(mesh.pos.x, mesh.pos.y, mesh.pos.z);
    mat4_t rotate_x_matrix = mat4_rotate_x(mesh.rot.x);
    mat4_t rotate_y_matrix = mat4_rotate_y(mesh.rot.y);
    mat4_t rotate_z_matrix = mat4_rotate_z(mesh.rot.z);

    // Loop through vertices and faces

    for(int i = 0; i < mesh.faces.size(); i++) {

        face_t current_face = mesh.faces[i];
        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[current_face.a - 1];   // our current face_t a index starts at 1, subtract 1 to compensate
        face_vertices[1] = mesh.vertices[current_face.b - 1];
        face_vertices[2] = mesh.vertices[current_face.c - 1];


        // Loop through all vertices of the current face, apply any transformations, and project the points

        vec4_t transformed_vertices[3];

        for(int j = 0; j < 3; j++) {

            vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

            // Use matrices for transformations instead of manual transformations
            // Create world matrix that combines scale, rotation, and translation matrices
            mat4_t world_matrix = mat4_identity();
            world_matrix = mat4_mul_mat(scale_matrix, world_matrix);
            world_matrix = mat4_mul_mat(rotate_x_matrix, world_matrix);
            world_matrix = mat4_mul_mat(rotate_y_matrix, world_matrix);
            world_matrix = mat4_mul_mat(rotate_z_matrix, world_matrix);
            world_matrix = mat4_mul_mat(translate_matrix, world_matrix);

            // Multiply world matrix by original vector
            transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

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
            vec3_t a = vec4_to_vec3(transformed_vertices[0]);
            vec3_t b = vec4_to_vec3(transformed_vertices[1]);
            vec3_t c = vec4_to_vec3(transformed_vertices[2]);

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

        if(cull % 2 == 0) {
            if(vec3_dot(norm, camera_ray) < 0) {
                continue;   // Bypass the rest of the for loop and restart
            }
        }

        vec4_t projected_points[3];
        for(int j = 0; j < 3; j++) {

            // Project current point
            projected_points[j] = mat4_persp_divide(proj_matrix, transformed_vertices[j]);

            // Scale the points into view
            projected_points[j].x *= win_width / 2.0;
            projected_points[j].y *= win_height / 2.0;

            // Translate projected point to the middle of the screen
            projected_points[j].x += win_width / 2.0;
            projected_points[j].y += win_height / 2.0;
        }

        // Calculate average depth of each face so we can sort and render back to front
        double avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0;

        uint32_t face_color = current_face.color;
        // Flat shading: color the triangle face based on the alignment of face normal and light ray
        face_color = shade_flat(norm, world_light, face_color);

        triangle_t projected_triangle = {
            .points = {
                { projected_points[0].x, projected_points[0].y },
                { projected_points[1].x, projected_points[1].y },
                { projected_points[2].x, projected_points[2].y }
            },
            .color = face_color,
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

    // draw_grid(60, 0xFF444444);  // Draw a gray grid

    // Loop through array and render all projected triangles
    int size = triangles_to_render.size();

    for(int i = 0; i < size; i++) {

        triangle_t triangle = triangles_to_render[i];

        if(control == 0 || control == 3 || control == 5 || control == 6 || control == 7) {
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

        if(control == 0 || control == 2 || control == 4 || control == 6 || control == 7) {
            // Draw triangle edges from vertices
            uint32_t edge_color;
            if(control == 2 || control == 4 || control == 8) {
                edge_color = white;
            }
            else {
                edge_color = black;
            }
            draw_triangle(
                triangle.points[0].x,
                triangle.points[0].y,
                triangle.points[1].x,
                triangle.points[1].y,
                triangle.points[2].x,
                triangle.points[2].y,
                edge_color
            );
        }

        if(control == 0 || control == 1 || control == 4 || control == 5 || control == 7) {
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

    string model;
    string color;
    string size;

    unordered_map<string, string> params;
    string key;
    string value;
    int len;
    cin >> len;

    for(int i = 0; i < len; i++) {
        cin >> key;
        cin >> value;
        params[key] = value;
    }

    for (auto const& p : params) {
        if(p.first == "Dimensions") size = p.second;
        if(p.first == "Colors") color = p.second;
        if(p.first == "Objects") model = p.second;
    }

    cout.flush();

    setup(model, color, size);
    // double sum;
    // double sub;
    // int i = 0;

    while(is_running) {
        // auto start = chrono::high_resolution_clock::now();

        process_input();
        update();
        render();

        // auto stop = chrono::high_resolution_clock::now();
        // auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        // if(i == 0) {
        //     sub = duration.count();
        // }
        // i++;
        // sum += duration.count();
        // cout << duration.count() << endl;
    }
    // cout << (sum - sub) / (pow(10, 6)) << " seconds" << endl;

    quit();
    free_resources();

    return 0;
}
