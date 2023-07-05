#include "font.h"

static int load(lua_State* L) {
    const char* path = lua_tostring(L, 1);
    int font_size = lua_tonumber(L, 2);

    TTF_Font* sdl_font = TTF_OpenFont(path, font_size);
    if (sdl_font == NULL) {
        fprintf(stderr, "Failed to load font '%s'.\n", path);
        exit(1);
    }

    Font* font = (Font*)lua_newuserdata(L, sizeof(Font));
    font->sdl_font = sdl_font;
    font->point_size = font_size;

    luaL_getmetatable(L, FONT_NAME);
    lua_setmetatable(L, -2);

    return 1;
}

static int close_font(lua_State* L) {
    Font* font = (Font*)luaL_checkudata(L, 1, FONT_NAME);
    if (font != NULL) {
        TTF_CloseFont(font->sdl_font);
    }
    return 0;
}

static const luaL_Reg font_lib[] = {
    { "load", load },
    { NULL, NULL },
};

void init_font_lib(lua_State *L) {
    luaL_newmetatable(L, FONT_NAME);
    lua_pushcfunction(L, close_font);
    lua_setfield(L, -2, "__gc");

    lua_getglobal(L, "point");
    luaL_newlib(L, font_lib);
    lua_setfield(L, -2, "font");
    lua_pop(L, 1);
}
