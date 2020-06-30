#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <SDL2/SDL.h>
#include "color.h"
#include "swap.h"
#include "vectors.h"
#include "textures.h"

// Global variables
// Extern: initialized outside of header file

// Frame Rate
#define FRAME_RATE (60)

// Frame Target Time Length
#define FRAME_TARGET_LENGTH (1000 / FRAME_RATE) // Milliseconds / Frame Rate

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

void draw_line(int x0, int y0, int x1, int y1, uint32_t color);

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void fill_flat_bottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void fill_flat_top(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void int_swap(int* a, int* b);

void draw_filled_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

void draw_texel(int x, int y, vec2_t a, vec2_t b, vec2_t c, uint32_t* texture);

void draw_textured_triangle(int x0, int y0, double u0, double v0, int x1, int y1, double u1, double v1, int x2, int y2, double u2, double v2, uint32_t* texture);

void draw_rect(int x, int y, int width, int height, uint32_t color);

void quit();

#endif
