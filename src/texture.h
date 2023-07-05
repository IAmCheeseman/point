#ifndef _point_texture_h
#define _point_texture_h

#include "common.h"
#include <SDL2/SDL_image.h>

#define TEXTURE_NAME "Texture"

typedef struct {
    SDL_Texture* sdl_texture;
    double angle;
    double scalex;
    double scaley;
    double pivotx;
    double pivoty;
    double quadx;
    double quady;
    double quadw;
    double quadh;
    bool fliph;
    bool flipv;
} Texture;

void init_texture_lib(lua_State *L, SDL_Renderer* sdl_renderer);

#endif