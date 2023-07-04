#ifndef _point_luahelpers_h
#define _point_luahelpers_h

#include "common.h"

bool call_lua_table_func(lua_State* L, int traceback_location, const char* table, const char* name, int return_count);
bool call_lua_func(lua_State* L, int traceback_location, int arg_count, int return_count);

#endif