#include "texture.h"

static SDL_Renderer *sdl_renderer;

static int load(lua_State* L) {
    const char* path = lua_tostring(L, 1);

    // Loading the image
    SDL_Surface* surf = IMG_Load(path);
    if (surf == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        exit(1);
    }
    SDL_Texture* sdl_texture = SDL_CreateTextureFromSurface(sdl_renderer, surf);
    if (sdl_texture == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_FreeSurface(surf);

    int w, h;
    SDL_QueryTexture(sdl_texture, NULL, NULL, &w, &h);

    // Creating the texture
    Texture* texture = (Texture*)lua_newuserdata(L, sizeof(Texture));
    texture->sdl_texture = sdl_texture;
    texture->angle = 0;
    texture->scalex = 1;
    texture->scaley = 1;
    texture->pivotx = floor(w / 2);
    texture->pivoty = floor(h / 2);
    texture->quadx = 0;
    texture->quady = 0;
    texture->quadw = w;
    texture->quadh = h;
    texture->fliph = false;
    texture->flipv = false;

    luaL_getmetatable(L, TEXTURE_NAME);
    lua_setmetatable(L, -2);

    return 1;
}

static int setscale(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);

    texture->scalex = x;
    texture->scaley = y;
    return 0;
}
static int setscalex(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    double x = lua_tonumber(L, 2);

    texture->scalex = x;
    return 0;
}
static int setscaley(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    double y = lua_tonumber(L, 2);

    texture->scaley = y;
    return 0;
}
static int getscale(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushnumber(L, texture->scalex);
    lua_pushnumber(L, texture->scaley);
    return 2;
}
static int getscalex(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushnumber(L, texture->scalex);
    return 1;
}
static int getscaley(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushnumber(L, texture->scaley);
    return 1;
}

static int setpivot(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);

    texture->pivotx = x;
    texture->pivoty = y;
    return 0;
}
static int setpivotx(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    double x = lua_tonumber(L, 2);

    texture->pivotx = x;
    return 0;
}
static int setpivoty(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    double y = lua_tonumber(L, 2);

    texture->pivoty = y;
    return 0;
}
static int getpivot(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushnumber(L, texture->pivotx);
    lua_pushnumber(L, texture->pivoty);
    return 2;
}
static int getpivotx(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushnumber(L, texture->pivotx);
    return 1;
}
static int getpivoty(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushnumber(L, texture->pivoty);
    return 1;
}

static int setfliph(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    bool flipped = lua_toboolean(L, 2);

    texture->fliph = flipped;
    return 0;
}
static int getfliph(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushboolean(L, texture->fliph);
    return 1;
}

static int setflipv(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    bool flipped = lua_toboolean(L, 2);

    texture->flipv = flipped;
    return 0;
}
static int getflipv(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushboolean(L, texture->flipv);
    return 1;
}

static int setangle(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    double angle = lua_tonumber(L, 2);

    texture->angle = angle;
    return 0;
}
static int getangle(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushnumber(L, texture->angle);
    return 1;
}

static int getquad(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    lua_pushnumber(L, texture->quadx);
    lua_pushnumber(L, texture->quady);
    lua_pushnumber(L, texture->quadw);
    lua_pushnumber(L, texture->quadh);
    return 4;
}
static int setquad(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);
    int x = lua_tonumber(L, 2);
    int y = lua_tonumber(L, 3);
    int w = lua_tonumber(L, 4);
    int h = lua_tonumber(L, 5);

    texture->quadx = x;
    texture->quady = y;
    texture->quadw = w;
    texture->quadh = h;
    return 0;
}

static int gettexturesize(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    int w, h;
    SDL_QueryTexture(texture->sdl_texture, NULL, NULL, &w, &h);

    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 2;
}

static int getwidth(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    int w;
    SDL_QueryTexture(texture->sdl_texture, NULL, NULL, &w, NULL);

    lua_pushinteger(L, w);
    return 1;
}
static int getheight(lua_State *L) {
    Texture *texture = (Texture *)luaL_checkudata(L, 1, TEXTURE_NAME);

    int h;
    SDL_QueryTexture(texture->sdl_texture, NULL, NULL, NULL, &h);

    lua_pushinteger(L, h);
    return 1;
}

static const luaL_Reg texture_lib[] = {
    {"load", load},

    {"setscale", setscale},
    {"getscale", getscale},
    {"setscalex", setscalex},
    {"getscalex", getscalex},
    {"setscaley", setscaley},
    {"getscaley", getscaley},

    {"setpivot", setpivot},
    {"getpivot", getpivot},
    {"setpivotx", setpivotx},
    {"getpivotx", getpivotx},
    {"setpivoty", setpivoty},
    {"getpivoty", getpivoty},

    {"setfliph", setfliph},
    {"getfliph", getfliph},

    {"setflipv", setflipv},
    {"getflipv", getflipv},

    {"setangle", setangle},
    {"getangle", getangle},

    {"setquad", setquad},
    {"getquad", getquad},

    {"getsize", gettexturesize},
    {"getheight", getheight},
    {"getwidth", getwidth},
    {NULL, NULL},
};

int destroy_texture(lua_State* L) {
    Texture* texture = (Texture*)luaL_checkudata(L, 1, TEXTURE_NAME);
    if (texture != NULL) {
        SDL_DestroyTexture(texture->sdl_texture);
    }
    return 0;
}

void init_texture_lib(lua_State *L, SDL_Renderer *renderer) {
    sdl_renderer = renderer;

    luaL_newmetatable(L, TEXTURE_NAME);
    lua_pushcfunction(L, destroy_texture);
    lua_setfield(L, -2, "__gc");

    lua_getglobal(L, "point");
    luaL_newlib(L, texture_lib);
    lua_setfield(L, -2, "texture");
    lua_pop(L, 1);
}
