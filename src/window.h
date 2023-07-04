#ifndef _point_window_h
#define _point_window_h

#include "common.h"

typedef struct {
    SDL_Window* sdl_window;
    bool should_close;
    bool is_fullscreen;
} Window;

void init_window_lib(lua_State* L, Window* engine_window);
void init_window(Window* window);
void close_window(Window* window);
void free_window(Window* window);

#endif