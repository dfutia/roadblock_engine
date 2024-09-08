#pragma once
#ifndef LUA_ENVIRONMENT_H
#define LUA_ENVIRONMENT_H

#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

extern "C" {
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

class Keyboard;
class Mouse;

class LuaEnvironment {
public:
	LuaEnvironment(Keyboard& kb, Mouse& mouse);
	~LuaEnvironment();

	sol::state& getState() { return lua; }
private:
	Keyboard& keyboard;
	Mouse& mouse;
	sol::state lua;

	void registerLua();
	void registerInput();
};


#endif 
