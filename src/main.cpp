#include "vectors.h"
#include "display.h"

bool is_running = false;

const int N_POINTS = 729;
vec3_t cube_points[N_POINTS];   // 9 x 9 x 9 cube
vec2_t projected_points[N_POINTS];  // 3D points projected on 2D screen

// Field of view factor to scale up points
float fov_factor = 128;

// Camera origin
vec3_t camera_position = { .x = 0, .y = 0, .z = -5 };

// View control
int view = 0;

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

    // Start loading array of vectors
    int i = 0;

    for(float x = -1; x <= 1; x += 0.25) {
        for(float y = -1; y <= 1; y += 0.25) {
            for(float z = -1; z <= 1; z += 0.25) {
                vec3_t point = { .x = x, .y = y, .z = z };
                // printf("%f %f %f\n", point.x, point.y, point.z);
                cube_points[i] = point;
                i++;
            }
        }
    }
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

            if(event.key.keysym.sym == SDLK_1) {    // Switch from perspective to ortho
                view = 1;
            }

            if(event.key.keysym.sym == SDLK_2) {    // Switch from perspective to ortho
                view = 2;
            }

            if(event.key.keysym.sym == SDLK_3) {    // Switch from perspective to ortho
                view = 3;
            }

            if(event.key.keysym.sym == SDLK_0) {    // Switch from perspective to ortho
                view = 0;
            }

            break;
    }
}

vec2_t project_ortho1(vec3_t point) {
    // Convert 3D point to 2D on x and y axis
    vec2_t projected_point = {
        .x = (fov_factor * point.x),    // Multiply by field ov view factor to scale the point up
        .y = (fov_factor * point.y)
    };

    return projected_point;
}

vec2_t project_ortho2(vec3_t point) {
    // Convert 3D point to 2D on x and z axis
    vec2_t projected_point = {
        .x = (fov_factor * point.x),    // Multiply by field ov view factor to scale the point up
        .y = (fov_factor * point.z)
    };

    return projected_point;
}

vec2_t project_ortho3(vec3_t point) {
    // Convert 3D point to 2D on y and z axis
    vec2_t projected_point = {
        .x = (fov_factor * point.y),    // Multiply by field ov view factor to scale the point up
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
    switch (view) {
        case 1: return project_ortho1(point);
        case 2: return project_ortho2(point);
        case 3: return project_ortho3(point);
        default: return project_persp(point);
    }
}

void update() {
    for(int i = 0; i < N_POINTS; i++) {

        // Receive a 3D point
        vec3_t point = cube_points[i];

        // Make things appear further away from camera
        // point.z -= camera_position.z;

        // Project the 3D point as a 2D point on a 2D screen
        vec2_t projected_point = project(view, point);

        // Save the 2D point to the projected points array
        projected_points[i] = projected_point;
    }
}

void render() {

    draw_grid(60, 0xFF606060);  // Draw a gray grid

    // Loop through all projected points and render them
    for(int i = 0; i < N_POINTS; i++) {
        vec2_t projected_point = projected_points[i];
        draw_rect(
            projected_point.x + win_width / 2, // Push rectangle to middle of screen
            projected_point.y + win_height / 2,
            4,
            4,
            0xFFFFFF00);
    }

    render_color_buffer();  // Render the color buffer texture

    clear_color_buffer(0xFF000000);   // Clear the color buffer to gray (hexa)

    SDL_RenderPresent(renderer);    // Draw on the renderer
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

    return 0;
}
