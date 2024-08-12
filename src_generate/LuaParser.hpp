#ifndef LUA_PARSER_HPP
#define LUA_PARSER_HPP

#include <string>


struct lua_State;

auto InitializeLua() -> lua_State*;
auto ExecuteLua(lua_State* const lua, const int width, const int height, const std::string &script) -> const void*;
void PopReturnLua(lua_State* const lua);
void ExitLua(lua_State* const lua);

#endif