#ifndef _point_events_h
#define _point_events_h

#include "common.h"

typedef struct {
    bool keys_pressed[SDL_NUM_SCANCODES];
    bool mouse_pressed[6];
    int mouse_x, mouse_y;
} EventHandler;

void init_event_handler(EventHandler* handler);
void handle_events(EventHandler* handler, lua_State* L, int traceback_index, SDL_Event event);
void init_event_libs(lua_State *L, EventHandler* event_handler);

#endif