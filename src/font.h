#ifndef _point_font_h
#define _point_font_h

#include "common.h"
#include "manager.h"
#include <SDL2/SDL_ttf.h>

typedef struct {
    TTF_Font* sdl_font;
} Font;

void init_font_manager(ItemManager* manager);
int create_font(ItemManager* manager, const char *path, int font_size);
void free_font(Font* font);
void free_font_manager(ItemManager* manager);
Font* font_get_at(ItemManager* manager, int index);

void init_font_lib(lua_State *L, ItemManager* font_manager);

#endif