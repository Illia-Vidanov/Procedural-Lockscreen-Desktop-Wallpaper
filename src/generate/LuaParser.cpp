#include "LuaParser.hpp"

#include <string>
#include <iostream>

#include "Lua.hpp"


auto InitializeLua() -> lua_State*
{
    lua_State *lua = luaL_newstate();

    // Disable garbage collector to initialize everything without it and clear everything at once
	lua_gc(lua, LUA_GCSTOP, 0);
	luaL_openlibs(lua);
	lua_gc(lua, LUA_GCRESTART, 0);
	
    return lua;
}

auto ExecuteLua(lua_State* const lua, const int width, const int height, const std::string &path) -> const void*
{
    luaL_loadfile(lua, path.c_str());
	
	lua_pushinteger(lua, width);
	lua_pushinteger(lua, height);
	
	lua_call(lua, 2, 1);

	if(lua_type(lua, -1) != LUA_TSTRING)
		std::cout << "Script returns not a string\n";

	const char *result = lua_tostring(lua, -1);

    return result;
}

void PopReturnLua(lua_State *const lua)
{
    lua_pop(lua, 1);
}

void ExitLua(lua_State* const lua)
{
    lua_close(lua);
}