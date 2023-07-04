#ifndef _point_renderer_h
#define _point_renderer_h

#include "common.h"
#include "window.h"

typedef struct {
    SDL_Renderer* sdl_renderer;
    SDL_Texture* target;
    bool is_rendering;
    bool is_pixel_perfect;
    int screen_width;
    int screen_height;
} Renderer;

void init_renderer(Renderer* renderer, Window* window);
void free_renderer(Renderer* renderer);
void set_screen_size(Renderer* renderer, int width, int height);

#endif