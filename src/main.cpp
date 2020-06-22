#include <iostream>
#include <SDL2/SDL.h>

// Global Variables
const int WIN_WIDTH = 960;
const int WIN_HEIGHT = 540;
SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
bool is_running = false;
// Color buffer has to be an unsigned int of 32 bits - 8 bits per color + alpha - 2^8 = 256
uint32_t* color_buffer = nullptr;

bool init_window() {

    // Try to initialize SDL with everything possible - check to see if there are errors
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        fprintf(stderr, "Error initializing SDL.\n");
        return false;
    }

    // Create SDL window
    window = SDL_CreateWindow(
        "Allegro",  // Title
        SDL_WINDOWPOS_CENTERED, // X Position
        SDL_WINDOWPOS_CENTERED, // Y Position
        WIN_WIDTH,    // Width
        WIN_HEIGHT,    // Height
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
    color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * WIN_WIDTH * WIN_HEIGHT);
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

void render() {

    SDL_SetRenderDrawColor( // Select the draw color
        renderer,   // Pointer to renderer
        255,    // Red value
        255,    // Green value
        255,    // Blue value
        255     // Alpha value
    );
    SDL_RenderClear(renderer);  // Clear the renderer
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

    return 0;
}
