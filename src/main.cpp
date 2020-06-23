#include "vectors.h"
#include "display.h"

bool is_running = false;

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

void render() {

    SDL_SetRenderDrawColor( // Select the draw color
        renderer,   // Pointer to renderer
        255,    // Red value
        255,    // Green value
        255,    // Blue value
        255     // Alpha value
    );

    SDL_RenderClear(renderer);  // Clear the renderer

    draw_grid(60, 0xFF606060);  // Draw a gray grid

    // draw_pixel(245, 425, 0xFFFFFFFF);   // Draw a white pixel

    // draw_rect(240, 180, 800, 600, 0xFFFF0000);  // Draw a red rectangle

    render_color_buffer();  // Render the color buffer texture

    clear_color_buffer(0xFF000000);   // Clear the color buffer to gray (hexa)

    SDL_RenderPresent(renderer);    // Draw on the renderer
}

int main() {

    // Create an SDL Window
    is_running = init_window();

    setup();
    vec3_t vec = { 2.0, 3.0, 1.0 };
    while(is_running) {
        process_input();
        update();
        render();
    }

    quit();

    return 0;
}
