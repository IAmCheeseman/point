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

static void process(EngineState* engine) {
    lua_getglobal(engine->L, "point");
    luaL_checktype(engine->L, -1, LUA_TTABLE);

    call_lua_table_func(engine->L, engine->traceback_location, "point", "onstep", 0);

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

        engine->prev = engine->now;
        engine->now = SDL_GetPerformanceCounter();
        engine->total_time += calculate_delta_time(engine);

        process(engine);

        draw(engine, x, y, w, h);
    }
}

int main(int argc, char* args[]) {
    if (argc == 2 && (strcmp(args[1], "--version") == 0 || strcmp(args[1], "-v") == 0)) {
        printf("Point v%s\n", VERSION_STR);
        exit(0);
    } else if (argc != 1) {
        printf("point help:\n");
        printf("\t-v, --version: returns point version.\n");
        exit(1);
    }

    EngineState* engine = create_engine();

    int res = luaL_dofile(engine->L, "main.lua");
    if (res != LUA_OK) {
        fprintf(stderr, "[LUA]:\n%s\n", lua_tostring(engine->L, -1));
        return 1;
    }

    call_lua_table_func(engine->L, engine->traceback_location, "point", "onload", 0);

    main_loop(engine);

    free_engine(engine);   

    return 0;
}