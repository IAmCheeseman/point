#include "engine.h"
#include "draw.h"
#include "window.h"
#include "vector.h"
#include "timing.h"
#include <SDL2/SDL_image.h>

static int traceback(lua_State *L)
{
    if (!lua_isstring(L, 1))
    { /* Non-string error object? Try metamethod. */
        if (lua_isnoneornil(L, 1) ||
            !luaL_callmeta(L, 1, "__tostring") ||
            !lua_isstring(L, -1))
            return 1;     /* Return non-string error object. */
        lua_remove(L, 1); /* Replace object by result of __tostring metamethod. */
    }
    luaL_traceback(L, L, lua_tostring(L, 1), 1);
    return 1;
}


EngineState *create_engine() {
    EngineState* engine = (EngineState*)malloc(sizeof(EngineState));

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        fprintf(stderr, "Failed to init SDL2: %s\n", SDL_GetError());
        exit(1);
    }
    if (IMG_Init(IMG_INIT_PNG) < 0) {
        fprintf(stderr, "Failed to init img: %s\n", IMG_GetError());
        exit(1);
    }

    engine->now = SDL_GetPerformanceCounter();
    engine->prev = 0;
    engine->total_time = 0;

    init_window(&engine->window);
    init_renderer(&engine->renderer, &engine->window);
    init_event_handler(&engine->event_handler);
    init_texture_manager(&engine->texture_manager);

    // Init lua
    engine->L = luaL_newstate();
    luaL_openlibs(engine->L);

    lua_pushcfunction(engine->L, traceback);
    engine->traceback_location = lua_gettop(engine->L);

    // Init libs
    lua_newtable(engine->L);
    
    init_draw_lib(engine->L, engine);
    init_window_lib(engine->L, &engine->window);
    init_event_libs(engine->L, &engine->event_handler);
    init_vector_lib(engine->L);
    init_texture_lib(engine->L, &engine->texture_manager, engine->renderer.sdl_renderer);
    init_time_lib(engine);

    lua_setglobal(engine->L, "point");

    luaL_dostring(engine->L, // Defining empty callbacks so lua doesn't get angry at me because they're nil
    "function point.onload() end\n"
    "function point.onstep(_) end\n"
    "function point.ondraw() end\n"
    "function point.onkeydown(_, _) end\n"
    "function point.onkeyup(_) end\n");

    return engine;
}

double get_screen_scale(EngineState* engine) {
    int sw = engine->renderer.screen_width;
    int sh = engine->renderer.screen_height;
    int ww, wh;
    SDL_GetWindowSize(engine->window.sdl_window, &ww, &wh);

    int w = ww + sw;
    int h = wh + sh;

    if (engine->renderer.is_pixel_perfect) {
        w -= w % sw;
        h -= h % sh;
    }

    while (w > ww)
        w -= sw;
    while (h > wh)
        h -= sh;

    return (double)w / (double)sw < (double)h / (double)sh
        ? (double)w / (double)sw
        : (double)h / (double)sh;

}

double calculate_delta_time(EngineState* engine) {
    return ((double)((engine->now - engine->prev) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001);
}

void free_engine(EngineState* engine) {
    lua_close(engine->L);
    free_window(&engine->window);
    free_renderer(&engine->renderer);
    free_texture_manager(&engine->texture_manager);

    engine->L = NULL;

    SDL_Quit();
}