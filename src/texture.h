#ifndef _point_texture_h
#define _point_texture_h

#include "common.h"
#include "manager.h"
#include <SDL2/SDL_image.h>

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

// typedef struct {
//     int texture_capacity;
//     int texture_count;
//     Texture** textures;
// } TextureManager;

void init_texture_manager(ItemManager* manager);
int create_texture(ItemManager* manager, SDL_Renderer* renderer, const char *path);
void free_texture(Texture* texture);
void free_texture_manager(ItemManager* manager);
Texture* texture_get_at(ItemManager* manager, int index);

void init_texture_lib(lua_State *L, ItemManager* texture_manager, SDL_Renderer* sdl_renderer);

#endif