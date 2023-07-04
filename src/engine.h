#ifndef _point_engine_h
#define _point_engine_h

#include "common.h"
#include "renderer.h"
#include "window.h"
#include "events.h"
#include "texture.h"

typedef struct {
    Window window;
    Renderer renderer;
    EventHandler event_handler;
    TextureManager texture_manager;
    lua_State* L;
    int traceback_location;
} EngineState;

EngineState* create_engine();
double get_screen_scale(EngineState* engine);
void free_engine(EngineState* engine);

#endif