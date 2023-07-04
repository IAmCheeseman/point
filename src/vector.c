#include <math.h>
#include "vector.h"

static double length(double x, double y) {
    return sqrt(pow(x, 2) + pow(y, 2));
}

static void normalize(double x, double y, double* out_x, double* out_y) {
    double l = length(x, y);
    if (l == 0) {
        *out_x = 0;
        *out_y = 0;
        return;
    }

    *out_x = x / l;
    *out_y = y / l;
}

static int vector_length(lua_State* L) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);

    lua_pushnumber(L, length(x, y));

    return 1;
}

static int vector_normalized(lua_State* L) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);

    double out_x, out_y;
    normalize(x, y, &out_x, &out_y);
    lua_pushnumber(L, out_x);
    lua_pushnumber(L, out_y);

    return 2;
}

// TODO: Add dot, angle, direction, distance, rotaion and lerp functions.

static const luaL_Reg vector[] = {
    { "length", vector_length },
    { "normalized", vector_normalized },
    { NULL, NULL },
};

void init_vector_lib(lua_State* L) {
    luaL_newlib(L, vector);
    lua_setfield(L, -2, "vector");
}
