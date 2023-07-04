#include "luahelpers.h"


bool call_lua_table_func(lua_State* L, int traceback_location, const char* table, const char* name, int return_count) {
    lua_getglobal(L, table);
    luaL_checktype(L, -1, LUA_TTABLE);

    lua_getfield(L, -1, name);
    luaL_checktype(L, -1, LUA_TFUNCTION);
    
    bool res = call_lua_func(L, traceback_location, 0, return_count);

    lua_pop(L, 1); // Table

    return res;
}

bool call_lua_func(lua_State* L, int traceback_location, int arg_count, int return_count) {
    int res = lua_pcall(L, arg_count, return_count, traceback_location);

    if (res && !lua_isnil(L, -1)) { 
        const char* error_message = lua_tostring(L, -1);
        if (error_message == NULL)
            error_message = "(error object is not a string.)";
        fprintf(stderr, "[LUA]:\n%s\n", error_message);
        lua_pop(L, 1);
        exit(1);
    }

    return true;
}