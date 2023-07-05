#include "timing.h"

EngineState* engine;

static int runtime(lua_State* L) {
    lua_pushnumber(L, engine->total_time);
    return 1;
}

static int delta(lua_State* L) {
    lua_pushnumber(L, calculate_delta_time(engine));
    return 1;
}

static const luaL_Reg timing[] = {
    { "runtime", runtime },
    { "delta", delta },
    { NULL, NULL },
};

void init_time_lib(EngineState* state) {
    engine = state;

    lua_getglobal(engine->L, "point");
    luaL_newlib(engine->L, timing);
    lua_setfield(engine->L, -2, "time");
    lua_pop(engine->L, 1);
}