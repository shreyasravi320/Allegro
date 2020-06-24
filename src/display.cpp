#include "display.h"

// Global variables
int win_width = 960;
int win_height = 540;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
uint32_t* color_buffer = nullptr;
SDL_Texture* color_buffer_texture = nullptr;

bool init_window() {

    // Try to initialize SDL with everything possible - check to see if there are errors
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {

        fprintf(stderr, "Error initializing SDL.\n");

        return false;
    }

    // Set window to full screen size
    SDL_DisplayMode display_mode;

    SDL_GetCurrentDisplayMode(0, &display_mode);

    win_width = display_mode.w;

    win_height = display_mode.h;

    // Create SDL window
    window = SDL_CreateWindow(
        "Allegro",  // Title
        SDL_WINDOWPOS_CENTERED, // X Position
        SDL_WINDOWPOS_CENTERED, // Y Position
        win_width,    // Width
        win_height,    // Height
        0   // Flags
    );

    // Check if pointer has value
    if(!window) {

        fprintf(stderr, "Error creating SDL window.\n");

        return false;
    }

    // Create SDL renderer
    renderer = SDL_CreateRenderer(
        window, // Pointer to window
        -1, // Display port code - -1 means choose the first one that works
        0   // Flags
    );

    // Check if pointer has value
    if(!renderer) {

        fprintf(stderr, "Error creating SDL renderer.\n");

        return false;
    }

    return true;
}

void clear_color_buffer(uint32_t clear_color) {

    // Clear the color buffer to the inputted color
    for(int i = 0; i < win_width * win_height; i++) {
        color_buffer[i] = clear_color;
    }
}

void render_color_buffer() {

    // Update the color buffer texture
    SDL_UpdateTexture(
        color_buffer_texture,   // Texture to apply
        nullptr,    // Rectangle for subdivisions
        color_buffer,   // Color to use for texture
        win_width * sizeof(uint32_t)    // Bytes needed for 1 row, aka pitch
    );

    // Render the color buffer texture
    SDL_RenderCopy(
        renderer,   // Pointer to renderer
        color_buffer_texture,   // Texture to copy
        nullptr,    // Source subdivisions
        nullptr // Destination subdivisions
    );
}

void draw_grid(int spacing, uint32_t color) {

    // Vertical and Horizontal lines spaced apart by input
    for(int i = 0; i < win_width; i++) {
        for(int j = 0; j < win_height; j++) {
            if(i % spacing == 0 || j % spacing == 0) {
                color_buffer[(win_width * j) + i] = color;
            }
        }
    }
}

void draw_pixel(int x, int y, uint32_t color) {

    // Draw a pixel based on given input for location and color
    if(x < win_width && x > -1 && y < win_height && y > -1) {
        color_buffer[(win_width * y) + x] = color;
    }
}

void draw_line(int x0, int y0, int x1, int y1, uint32_t color) {

    // Implementation of DDA Line Drawing algorithm - replace with Bresenham's for speed
    int x = x1 - x0;
    int y = y1 - y0;

    int side_length = (abs(x) >= abs(y)) ? abs(x) : abs(y);     // side length = abs(x) if abs(x) >= abs(y) otherwise it equals abs(y)

    float dx = x / (float) side_length;
    float dy = y / (float) side_length;

    float current_x = x0;
    float current_y = y0;

    for(int i = 0; i <= side_length; i++) {
        draw_pixel(round(current_x), round(current_y), color);  // Round because we can't have decimal pixels
        current_x += dx;
        current_y += dy;
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {

    // Draw a triangle based on given input of 3 points and color
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

void draw_rect(int x, int y, int width, int height, uint32_t color) {

    // Draw a rectangle based on given input for location, size, and color
    for(int i = x; i < x + width; i++) {
        for(int j = y; j < y + height; j++) {
            draw_pixel(i, j, color);
        }
    }
}

void quit() {

    // Actions to perform when program quits
    free(color_buffer); // Free memory stored in color buffer

    SDL_DestroyRenderer(renderer);  // Destroy renderer

    SDL_DestroyWindow(window);  // Destroy window

    SDL_Quit(); // Uninitialize SDL
}
