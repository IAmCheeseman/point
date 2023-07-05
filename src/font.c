#include "font.h"

void init_font_manager(ItemManager* manager) {
    ITEM_MANAGER_INITIALIZE(manager, Font);
}

int create_font(ItemManager* manager, const char *path, int font_size) {
    ITEM_MANAGER_RESIZE(manager, Font);

    Font* font = (Font*)malloc(sizeof(Font));
    font->sdl_font = TTF_OpenFont(path, font_size);
    font->point_size = font_size;

    if (font->sdl_font == NULL) {
        fprintf(stderr, "Failed to load font '%s'.\n", path);
        exit(1);
    }

    return item_manager_set(manager, font);
}

void free_font(Font* font) {
    TTF_CloseFont(font->sdl_font);
    font->sdl_font = NULL;
    free(font);
}

void free_font_manager(ItemManager* manager) {
    ITEM_MANAGER_FREE(free_font, Font);
}

Font* font_get_at(ItemManager* manager, int index) {
    return (Font*)item_manager_get(manager, index, "font");
}

static ItemManager* manager;

static int load(lua_State* L) {
    const char* path = lua_tostring(L, 1);
    int font_size = lua_tonumber(L, 2);
    int index = create_font(manager, path, font_size);
    if (index == -1) {
        fprintf(stderr, "Invalid font index '-1'.\n");
        exit(1);
    }
    lua_pushnumber(L, index);

    return 1;
}

static const luaL_Reg font_lib[] = {
    { "load", load },
    { NULL, NULL },
};

void init_font_lib(lua_State *L, ItemManager* font_manager) {
    manager = font_manager;

    lua_getglobal(L, "point");
    luaL_newlib(L, font_lib);
    lua_setfield(L, -2, "font");
    lua_pop(L, 1);
}
