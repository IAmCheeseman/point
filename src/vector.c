#include <math.h>
#include "vector.h"

static double length(double x, double y) {
    return sqrt(pow(x, 2) + pow(y, 2));
}

static double angle(double x, double y) {
    double angle = atan2(y, x);
    if (angle < 0) angle += M_PI * 2;
    return angle;
}

static double dot(double x, double y, double xx, double yy) {
    return x * xx + y * yy;
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

static int vector_dot(lua_State* L) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);
    double xx = lua_tonumber(L, 3);
    double yy = lua_tonumber(L, 4);

    lua_pushnumber(L, dot(x, y, xx, yy));

    return 1;
}

static int vector_directionto(lua_State* L) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);
    double xx = lua_tonumber(L, 3);
    double yy = lua_tonumber(L, 4);

    double out_x, out_y;
    normalize(xx - x, yy - y, &out_x, &out_y);
    lua_pushnumber(L, out_x);
    lua_pushnumber(L, out_y);
    
    return 2;
}

static int vector_distanceto(lua_State* L) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);
    double xx = lua_tonumber(L, 3);
    double yy = lua_tonumber(L, 4);

    double distance = length(xx - x, yy - y);
    lua_pushnumber(L, distance);
    
    return 1;
}

static int vector_manhattandistanceto(lua_State* L) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);
    double xx = lua_tonumber(L, 3);
    double yy = lua_tonumber(L, 4);

    lua_pushnumber(L, abs(x - xx) + abs(y - yy));
    
    return 1;
}

static int vector_angle(lua_State* L) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);

    lua_pushnumber(L, angle(x, y));

    return 1;
}

static int vector_angleto(lua_State* L) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);
    double xx = lua_tonumber(L, 3);
    double yy = lua_tonumber(L, 4);

    lua_pushnumber(L, angle(xx - x, yy - y));

    return 1;
}

static int vector_rotated(lua_State* L) {
    double x = lua_tonumber(L, 1);
    double y = lua_tonumber(L, 2);
    double r = lua_tonumber(L, 3);

    double new_rot = angle(x, y) + r;
    double l = length(x, y);

    lua_pushnumber(L, cos(new_rot) * l);
    lua_pushnumber(L, sin(new_rot) * l);

    return 2;
}

// TODO: Add dot, angle, direction, distance, rotaion and lerp functions.

static const luaL_Reg vector[] = {
    { "length", vector_length },
    { "normalized", vector_normalized },
    { "dot", vector_dot },
    { "directionto", vector_directionto },
    { "distanceto", vector_distanceto },
    { "manhattandistanceto", vector_manhattandistanceto },
    { "angle", vector_angle },
    { "angleto", vector_angleto },
    { "rotated", vector_rotated },
    { NULL, NULL },
};

void init_vector_lib(lua_State* L) {
    luaL_newlib(L, vector);
    lua_setfield(L, -2, "vector");
}
