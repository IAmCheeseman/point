#include "texture.h"

void init_texture_manager(ItemManager* manager) {
    ITEM_MANAGER_INITIALIZE(manager, Texture);
}

int create_texture(ItemManager* manager, SDL_Renderer* renderer, const char *path) {
    ITEM_MANAGER_RESIZE(manager, Texture);

    // Loading the image
    SDL_Surface* surf = IMG_Load(path);
    if (surf == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", IMG_GetError());
        exit(1);
    }
    SDL_Texture* sdl_texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (sdl_texture == NULL) {
        fprintf(stderr, "Failed to load image: %s\n", SDL_GetError());
        exit(1);
    }

    SDL_FreeSurface(surf);

    int w, h;
    SDL_QueryTexture(sdl_texture, NULL, NULL, &w, &h);

    // Creating the texture
    Texture* texture = (Texture*)malloc(sizeof(Texture));
    texture->sdl_texture = sdl_texture;
    texture->angle = 0;
    texture->scalex = 1;
    texture->scaley = 1;
    texture->centerx = 0;
    texture->centery = 0;
    texture->quadx = 0;
    texture->quady = 0;
    texture->quadw = w;
    texture->quadh = h;
    texture->fliph = false;
    texture->flipv = false;

    return item_manager_set(manager, texture);
}

void free_texture(Texture* texture) {
    SDL_DestroyTexture(texture->sdl_texture);
    texture->sdl_texture = NULL;
    free(texture);
}

void free_texture_manager(ItemManager* manager) {
    ITEM_MANAGER_FREE(free_texture, Texture);
}

Texture* texture_get_at(ItemManager* manager, int index) {
    return (Texture*)item_manager_get(manager, index, "texture");
}

ItemManager* manager;
SDL_Renderer* sdl_renderer;

static int load(lua_State* L) {
    const char* path = lua_tostring(L, 1);
    int index = create_texture(manager, sdl_renderer, path);
    if (index == -1) {
        fprintf(stderr, "Invalid texture index '-1'.\n");
        exit(1);
    }
    lua_pushnumber(L, index);

    return 1;
}

static int isvalid(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    lua_pushboolean(L, item_manager_index_is_valid(manager, texture_index));
    return 1;
}

static int free_lua_texture(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);
    free_texture(texture);
    manager->items[texture_index] = NULL;

    return 0;
}

static int setscale(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->scalex = x;
    texture->scaley = y;
    return 0;
}
static int setscalex(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    double x = lua_tonumber(L, 2);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->scalex = x;
    return 0;
}
static int setscaley(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->scaley = y;
    return 0;
}
static int getscale(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushnumber(L, texture->scalex);
    lua_pushnumber(L, texture->scaley);
    return 2;
}
static int getscalex(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushnumber(L, texture->scalex);
    return 1;
}
static int getscaley(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushnumber(L, texture->scaley);
    return 1;
}

static int setcenter(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    double x = lua_tonumber(L, 2);
    double y = lua_tonumber(L, 3);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->centerx = x;
    texture->centery = y;
    return 0;
}
static int setcenterx(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    double x = lua_tonumber(L, 2);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->centerx = x;
    return 0;
}
static int setcentery(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->centery = y;
    return 0;
}
static int getcenter(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushnumber(L, texture->centerx);
    lua_pushnumber(L, texture->centery);
    return 2;
}
static int getcenterx(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushnumber(L, texture->centerx);
    return 1;
}
static int getcentery(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushnumber(L, texture->centery);
    return 1;
}

static int setfliph(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    bool flipped = lua_toboolean(L, 2);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->fliph = flipped;
    return 0;
}
static int getfliph(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushboolean(L, texture->fliph);
    return 1;
}

static int setflipv(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    bool flipped = lua_toboolean(L, 2);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->flipv = flipped;
    return 0;
}
static int getflipv(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushboolean(L, texture->flipv);
    return 1;
}

static int setangle(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    double angle = lua_tonumber(L, 2);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->angle = angle;
    return 0;
}
static int getangle(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushnumber(L, texture->angle);
    return 1;
}

static int getquad(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    lua_pushnumber(L, texture->quadx);
    lua_pushnumber(L, texture->quady);
    lua_pushnumber(L, texture->quadw);
    lua_pushnumber(L, texture->quadh);
    return 4;
}
static int setquad(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    int x = lua_tonumber(L, 2);
    int y = lua_tonumber(L, 3);
    int w = lua_tonumber(L, 4);
    int h = lua_tonumber(L, 5);
    Texture* texture = texture_get_at(manager, texture_index);

    texture->quadx = x;
    texture->quady = y;
    texture->quadw = w;
    texture->quadh = h;
    return 0;
}

static int gettexturesize(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    int w, h;
    SDL_QueryTexture(texture->sdl_texture, NULL, NULL, &w, &h);

    lua_pushinteger(L, w);
    lua_pushinteger(L, h);
    return 2;
}

static int getwidth(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    int w;
    SDL_QueryTexture(texture->sdl_texture, NULL, NULL, &w, NULL);

    lua_pushinteger(L, w);
    return 1;
}
static int getheight(lua_State* L) {
    int texture_index = lua_tonumber(L, 1);
    Texture* texture = texture_get_at(manager, texture_index);

    int h;
    SDL_QueryTexture(texture->sdl_texture, NULL, NULL, NULL, &h);

    lua_pushinteger(L, h);
    return 1;
}

static const luaL_Reg texture_lib[] = {
    { "load", load },
    { "free", free_lua_texture },
    { "isvalid", isvalid },

    { "setscale",   setscale },
    { "getscale",   getscale },
    { "setscalex",  setscalex },
    { "getscalex",  getscalex },
    { "setscaley",  setscaley },
    { "getscaley",  getscaley },

    { "setcenter",  setcenter },
    { "getcenter",  getcenter },
    { "setcenterx", setcenterx },
    { "getcenterx", getcenterx },
    { "setcentery", setcentery },
    { "getcentery", getcentery },

    { "setfliph",   setfliph },
    { "getfliph",   getfliph },

    { "setflipv",   setflipv },
    { "getflipv",   getflipv },

    { "setangle",   setangle },
    { "getangle",   getangle },

    { "setquad",    setquad },
    { "getquad",    getquad },

    { "getsize",    gettexturesize },
    { "getheight",  getheight },
    { "getwidth",   getwidth },
    { NULL, NULL },
};

void init_texture_lib(lua_State *L, ItemManager* texture_manager, SDL_Renderer* renderer) {
    manager = texture_manager;
    sdl_renderer = renderer;

    luaL_newlib(L, texture_lib);
    lua_setfield(L, -2, "texture");
}
