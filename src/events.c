#include "events.h"
#include "keymappings.h"
#include "luahelpers.h"

void init_event_handler(EventHandler* handler) {
    for (int i = 0; i < SDL_NUM_SCANCODES; i++)
        handler->keys_pressed[i] = false;
}

void handle_events(EventHandler* handler, lua_State* L, int traceback_index, SDL_Event event) {
    lua_getglobal(L, "point");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        fprintf(stderr, "No point table.\n");
        exit(1);
    }

    switch (event.type) {
        case SDL_KEYDOWN: {
            bool is_repeat = handler->keys_pressed[event.key.keysym.scancode];
            handler->keys_pressed[event.key.keysym.scancode] = true;
            
            lua_getfield(L, -1, "onkeydown");
            luaL_checktype(L, -1, LUA_TFUNCTION);

            lua_pushinteger(L, event.key.keysym.scancode);
            lua_pushboolean(L, is_repeat);
            call_lua_func(L, traceback_index, 2, 0);
            break;
        }
        case SDL_KEYUP: {
            handler->keys_pressed[event.key.keysym.scancode] = false;
            lua_getfield(L, -1, "onkeyup");
            luaL_checktype(L, -1, LUA_TFUNCTION);
        
            lua_pushinteger(L, event.key.keysym.scancode);
            call_lua_func(L, traceback_index, 1, 0);

            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            handler->mouse_pressed[event.button.button] = true;
            break;
        }
        case SDL_MOUSEBUTTONUP: {
            handler->mouse_pressed[event.button.button] = false;
            break;
        }
        case SDL_MOUSEMOTION: {
            handler->keys_pressed[event.key.keysym.scancode] = false;
            lua_getfield(L, -1, "onmousemove");
            luaL_checktype(L, -1, LUA_TFUNCTION);
        
            lua_pushinteger(L, handler->mouse_x);
            lua_pushinteger(L, handler->mouse_y);
            lua_pushinteger(L, event.motion.xrel);
            lua_pushinteger(L, event.motion.yrel);
            call_lua_func(L, traceback_index, 4, 0);

            break;
        }
        case SDL_WINDOWEVENT: {
            switch (event.window.event) {
                case SDL_WINDOWEVENT_FOCUS_GAINED: {
                    lua_getfield(L, -1, "onwindowfocused");
                    luaL_checktype(L, -1, LUA_TFUNCTION);
                
                    call_lua_func(L, traceback_index, 0, 0);

                    break;
                }
                case SDL_WINDOWEVENT_FOCUS_LOST: {
                    lua_getfield(L, -1, "onwindowunfocused");
                    luaL_checktype(L, -1, LUA_TFUNCTION);
                
                    call_lua_func(L, traceback_index, 0, 0);

                    break;
                }
                case SDL_WINDOWEVENT_RESIZED: {
                    lua_getfield(L, -1, "onwindowresized");
                    luaL_checktype(L, -1, LUA_TFUNCTION);

                    lua_pushinteger(L, event.window.data1);
                    lua_pushinteger(L, event.window.data2);
                    call_lua_func(L, traceback_index, 2, 0);

                    break;
                }
                case SDL_WINDOWEVENT_MOVED: {
                    lua_getfield(L, -1, "onwindowmoved");
                    luaL_checktype(L, -1, LUA_TFUNCTION);

                    lua_pushinteger(L, event.window.data1);
                    lua_pushinteger(L, event.window.data2);
                    call_lua_func(L, traceback_index, 2, 0);

                    break;
                }
            }
        }
    }

    lua_pop(L, 1); // Table
}

EventHandler* handler;

static int iskeydown(lua_State* L) {
    int keycode = lua_tointeger(L, 1);
    lua_pushboolean(L, handler->keys_pressed[keycode]);

    return 1;
}

static int ismousedown(lua_State* L) {
    int button = lua_tointeger(L, 1);
    button = button < 1 ? 1 : button;
    button = button > 5 ? 5 : button;
    lua_pushboolean(L, handler->mouse_pressed[button]);

    return 1;
}

static int getmousepos(lua_State* L) {
    lua_pushinteger(L, handler->mouse_x);
    lua_pushinteger(L, handler->mouse_y);

    return 2;
}

static int getglobalmousepos(lua_State* L) {
    int x, y;
    SDL_GetGlobalMouseState(&x, &y);

    lua_pushinteger(L, x);
    lua_pushinteger(L, y);

    return 2;
}

static const luaL_Reg input[] = {
    { "getmousepos", getmousepos },
    { "getglobalmousepos", getglobalmousepos },
    { "iskeydown", iskeydown },
    { "ismousedown", ismousedown },
    { NULL, NULL },
};

void init_event_libs(lua_State *L, EventHandler* event_handler) {
    handler = event_handler;

    lua_getglobal(L, "point");
    luaL_newlib(L, input);
    lua_setfield(L, -2, "input");

    // Scancode variables
    lua_newtable(L);
    for (int i = 0; i < SDL_NUM_SCANCODES; i++) {
        if (scancode_names[i] == NULL)
            continue;
        
        lua_pushinteger(L, i);
        lua_setfield(L, -2, scancode_names[i]);
    }
    lua_setfield(L, -2, "key");

    // Mouse button variables
    lua_newtable(L);
    for (int i = 0; i < 6; i++) {
        if (mouse_button_names[i] == NULL)
            continue;
        
        lua_pushinteger(L, i);
        lua_setfield(L, -2, mouse_button_names[i]);
    }
    lua_setfield(L, -2, "mouse");

    lua_pop(L, 1);
}