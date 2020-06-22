#include <iostream>
#include <SDL2/SDL.h>

// Global Variables
int win_width = 960;
int win_height = 540;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool is_running = false;

// Color buffer has to be an unsigned int of 32 bits - 8 bits per color + alpha - 2^8 = 256
// Hexadecimal = 0x[Alpha][Red][Green][Blue] - Red = 0xFFFF0000 - F = full
uint32_t* color_buffer = nullptr;

// Color buffer texture to apply the color buffer to the renderer
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
            break;
    }
}

void update() {

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

void render() {

    SDL_SetRenderDrawColor( // Select the draw color
        renderer,   // Pointer to renderer
        255,    // Red value
        255,    // Green value
        255,    // Blue value
        255     // Alpha value
    );
    SDL_RenderClear(renderer);  // Clear the renderer
    render_color_buffer();  // Render the color buffer texture
    clear_color_buffer(0xFFFF0000);   // Clear the color buffer to white (hexa)
    SDL_RenderPresent(renderer);    // Draw on the renderer
}

void quit() {

    // Actions to perform when program quits
    free(color_buffer); // Free memory stored in color buffer
    SDL_DestroyRenderer(renderer);  // Destroy renderer
    SDL_DestroyWindow(window);  // Destroy window
    SDL_Quit(); // Uninitialize SDL
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
