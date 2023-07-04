#include "window.h"

void init_window(Window *window) {
    window->should_close = false;
    window->is_fullscreen = false;
    window->sdl_window = SDL_CreateWindow(
        "PointEngine",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        400, 400,
        SDL_WINDOW_RESIZABLE);
    
    if (window->sdl_window == NULL) {
        fprintf(stderr, "Could not init window: %s", SDL_GetError());
        exit(1);
    }
}

void close_window(Window* window) {
    window->should_close = true;
}

void free_window(Window* window) {
    SDL_DestroyWindow(window->sdl_window);
    window->sdl_window = NULL;
}

Window* window;

static int settitle(lua_State* L) {
    const char* title = lua_tostring(L, 1);
    SDL_SetWindowTitle(window->sdl_window, title);

    return 0;
}

static int gettitle(lua_State* L) {
    const char* title = SDL_GetWindowTitle(window->sdl_window);
    lua_pushstring(L, title);

    return 1;
}

static int getpos(lua_State* L) {
    int x, y;
    SDL_GetWindowPosition(window->sdl_window, &x, &y);
    
    lua_pushinteger(L, x);
    lua_pushinteger(L, y);

    return 2;
}

static int setpos(lua_State* L) {
    int x = lua_tointeger(L, 1);
    int y = lua_tointeger(L, 2);
    SDL_SetWindowPosition(window->sdl_window, x, y);

    return 0;
}

static int getsize(lua_State* L) {
    int w, h;
    SDL_GetWindowSize(window->sdl_window, &w, &h);
    
    lua_pushinteger(L, w);
    lua_pushinteger(L, h);

    return 2;
}

static int setsize(lua_State* L) {
    int w = lua_tointeger(L, 1);
    int h = lua_tointeger(L, 2);
    SDL_SetWindowSize(window->sdl_window, w, h);

    return 0;
}

static int exit_window(lua_State* L) {
    window->should_close = true;

    return 0;
}

static int center(lua_State* L) {
    SDL_SetWindowPosition(window->sdl_window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    return 0;
}

static int setfullscreen(lua_State* L) {
    bool in_fullscreen = lua_toboolean(L, 1);
    const char* mode = luaL_optstring(L, 2, "desktop");

    SDL_WindowFlags flag = 0;
    if (in_fullscreen) {
        if (strcmp(mode, "fullscreen") == 0) {
            flag = SDL_WINDOW_FULLSCREEN;
        } else if (strcmp(mode, "desktop") == 0) {
            flag = SDL_WINDOW_FULLSCREEN_DESKTOP;
        } else {
            fprintf(stderr, "Only 'fullscreen' and 'desktop' are valid fullscreen modes.\n");
            exit(1);
        }
    }

    SDL_SetWindowFullscreen(window->sdl_window, flag);
    window->is_fullscreen = in_fullscreen;
    return 0;
}

static int getfullscreen(lua_State* L) {
    lua_pushboolean(L, window->is_fullscreen);
    return 1;
}

static const luaL_Reg window_lib[] = {
    { "exit", exit_window },
    { "center", center },
    { "settitle", settitle },
    { "gettitle", gettitle },
    { "getpos", getpos },
    { "setpos", setpos },
    { "getsize", getsize },
    { "setsize", setsize },
    { "getfullscreen", getfullscreen },
    { "setfullscreen", setfullscreen },
    { NULL, NULL },
};

void init_window_lib(lua_State* L, Window* engine_window) {
    window = engine_window;

    // lua_getglobal(L, "point");
    luaL_newlib(L, window_lib);
    lua_setfield(L, -2, "window");
}
