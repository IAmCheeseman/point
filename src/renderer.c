#include "renderer.h"
#include "window.h"

void init_renderer(Renderer* renderer, Window* window) {
    SDL_GetWindowSize(window->sdl_window, &renderer->screen_width, &renderer->screen_height);
    renderer->sdl_renderer = SDL_CreateRenderer(
        window->sdl_window, 
        -1, SDL_RENDERER_ACCELERATED);
    
    if (renderer->sdl_renderer == NULL) {
        fprintf(stderr, "Could not init renderer: %s", SDL_GetError());
    }

    renderer->is_rendering = false;
    renderer->is_pixel_perfect = false;

    SDL_GetWindowSize(window->sdl_window, &renderer->screen_width, &renderer->screen_height);
    set_screen_size(renderer, renderer->screen_width, renderer->screen_height);
}

void free_renderer(Renderer* renderer) {
    SDL_DestroyRenderer(renderer->sdl_renderer);
    renderer->sdl_renderer = NULL;

    if (renderer->target != NULL) {
        SDL_DestroyTexture(renderer->target);
        renderer->target = NULL;
    }
}


void set_screen_size(Renderer* renderer, int width, int height) {
    renderer->screen_width = width;
    renderer->screen_height = height;

    if (renderer->target != NULL) {
        SDL_DestroyTexture(renderer->target);
    }

    renderer->target = SDL_CreateTexture(
        renderer->sdl_renderer, 
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 
        width, height);
    
    if (renderer->target == NULL) {
        fprintf(stderr, "Failed to create render target: %s\n", SDL_GetError());
        exit(1);
    }
}