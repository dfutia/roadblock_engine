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

class LuaEnvironment {
public:
	LuaEnvironment();
private:
	sol::state lua;
	void doBindings();
};

#endif 
