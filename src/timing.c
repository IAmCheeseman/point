#include "timing.h"

EngineState* engine;

static int timerunning(lua_State* L) {
    lua_pushnumber(L, engine->total_time);
    return 1;
}

static int deltatime(lua_State* L) {
    lua_pushnumber(L, calculate_delta_time(engine));
    return 1;
}

static const luaL_Reg timing[] = {
    { "timerunning", timerunning },
    { "deltatime", deltatime },
    { NULL, NULL },
};

void init_time_lib(EngineState* state) {
    engine = state;

    luaL_newlib(engine->L, timing);
    lua_setfield(engine->L, -2, "time");
}