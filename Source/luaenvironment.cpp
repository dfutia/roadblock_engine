#include "luaenvironment.h"
#include "instance.h"

LuaEnvironment::LuaEnvironment() {
	lua.open_libraries(sol::lib::base, sol::lib::package);

	doBindings();

	std::string scriptPath = "./Game/?.lua";
	lua["package"]["path"] = scriptPath;
	lua.script("require('main')");
}

void LuaEnvironment::doBindings() {
	lua.new_usertype<Instance>("Instance",
		"new", sol::no_constructor,
		"SetParent", &Instance::setParent,
		"GetChildren", &Instance::getChildren,
		"GetDescendants", &Instance::getDescendants
	);

	lua.new_usertype<Model>("Model",
		"new", sol::factories([]() { return new Model(); }),
		sol::base_classes, sol::bases<Instance>()
	);

	lua.new_usertype<Part>("Part",
		"new", sol::factories([]() { return new Part(); }),
		sol::base_classes, sol::bases<Instance>()
	);
	
	lua["Instance"] = lua.create_table_with(
		"new", [](const std::string& className) -> Instance* {
			if (className == "Model") return new Model();
			if (className == "Part") return new Part();
			return nullptr;
		}
	);
}
