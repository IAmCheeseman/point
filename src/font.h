#ifndef _point_font_h
#define _point_font_h

#include "common.h"
#include <SDL2/SDL_ttf.h>

#define FONT_NAME "Font"

typedef struct {
    TTF_Font* sdl_font;
    int point_size;
} Font;

void init_font_lib(lua_State *L);
 
#endif