#include <time.h>

#include "common.h"
#include "engine.h"
#include "luahelpers.h"

static void process_events(EngineState* engine) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: {
                engine->window.should_close = true;
                break;
            }
            default: {
                handle_events(&engine->event_handler, engine->L, engine->traceback_location, event);
                break;
            }
        }
    }
}

static void process(EngineState* engine, double delta) {
    lua_getglobal(engine->L, "point");
    luaL_checktype(engine->L, -1, LUA_TTABLE);

    lua_getfield(engine->L, -1, "onstep");
    luaL_checktype(engine->L, -1, LUA_TFUNCTION);
    lua_pushnumber(engine->L, delta);
    call_lua_func(engine->L, engine->traceback_location, 1, 0);

    lua_pop(engine->L, 1); // Table
}

static void draw(EngineState* engine, int x, int y, int w, int h) {
    // Start rendering
    engine->renderer.is_rendering = true;
    SDL_SetRenderTarget(engine->renderer.sdl_renderer, engine->renderer.target);

    // Render game
    SDL_SetRenderDrawColor(engine->renderer.sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(engine->renderer.sdl_renderer);
    SDL_SetRenderDrawColor(engine->renderer.sdl_renderer, 255, 255, 255, 255);

    call_lua_table_func(engine->L, engine->traceback_location, "point", "ondraw", 0);

    SDL_SetRenderTarget(engine->renderer.sdl_renderer, NULL);
    // Render target texture
    SDL_SetRenderDrawColor(engine->renderer.sdl_renderer, 0, 0, 0, 255);
    SDL_RenderClear(engine->renderer.sdl_renderer);

    SDL_Rect dst = (SDL_Rect) { x, y, w, h };
    SDL_RenderCopy(engine->renderer.sdl_renderer, engine->renderer.target, NULL, &dst);

    SDL_RenderPresent(engine->renderer.sdl_renderer);
    engine->renderer.is_rendering = false;
}

static void main_loop(EngineState* engine) {
    double now = SDL_GetPerformanceCounter();
    double prev = 0;
    while (!engine->window.should_close) {
        process_events(engine);
        
        // Calculate where the target texture goes
        int ww, wh;
        SDL_GetWindowSize(engine->window.sdl_window, &ww, &wh);
        double scale = get_screen_scale(engine);

        int w = (int)(engine->renderer.screen_width * scale);
        int h = (int)(engine->renderer.screen_height * scale);
        int x = (ww - w) / 2;
        int y = (wh - h) / 2;

        // Calculating mouse position ahead of time because of C things
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        engine->event_handler.mouse_x = (mx - x) / scale;
        engine->event_handler.mouse_y = (my - y) / scale;

        // Why is dt always SO cringe to calculate!
        prev = now;
        now = SDL_GetPerformanceCounter();
        double delta = ((double)((now - prev) * 1000 / (double)SDL_GetPerformanceFrequency()) * 0.001);

        process(engine, delta);
        draw(engine, x, y, w, h);
    }
}

int main(int argc, char* args[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [directory]\n", args[0]);
        return 1;
    }

    EngineState* engine = create_engine();

    char* path = (char*)malloc(sizeof(char) * 100);
    strcpy(path, args[1]);
    if (path[strlen(path) - 1] != '/') {
        strcat(path, "/");
    }
    strcat(path, "main.lua");
    printf("%s\n", path);


    int res = luaL_dofile(engine->L, path);
    free(path);

    if (res != LUA_OK) {
        fprintf(stderr, "[LUA]:\n%s\n", lua_tostring(engine->L, -1));
        return 1;
    }

    call_lua_table_func(engine->L, engine->traceback_location, "point", "onload", 0);

    main_loop(engine);

    free_engine(engine);   

    return 0;
}