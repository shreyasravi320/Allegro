#ifndef DISPLAY_H
#define DISPLAY_H

#include <SDL2/SDL.h>
#include "vectors.h"

// Global variables
// Extern: initialized outside of header file
extern int win_width;
extern int win_height;
extern SDL_Window* window;
extern SDL_Renderer* renderer;

// Color buffer has to be an unsigned int of 32 bits - 8 bits per color + alpha - 2^8 = 256
// Hexadecimal = 0x[Alpha][Red][Green][Blue] - Red = 0xFFFF0000 - F = full
extern uint32_t* color_buffer;

// Color buffer texture to apply the color buffer to the renderer
extern SDL_Texture* color_buffer_texture;

bool init_window();

void clear_color_buffer(uint32_t clear_color);

void render_color_buffer();

void draw_grid(int spacing, uint32_t color);

void draw_pixel(int x, int y, uint32_t color);

void draw_rect(int x, int y, int width, int height, uint32_t color);

void quit();

#endif
