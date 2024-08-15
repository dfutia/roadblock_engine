#include "Script/luaenvironment.h"
#include "Event/engineevents.h"
#include "Common/instance.h"
#include "Common/model.h"
#include "Common/part.h"
#include "Common/script.h"

LuaEnvironment::LuaEnvironment() {
	lua.open_libraries(sol::lib::base, sol::lib::package);

	auto argsToString = [](sol::variadic_args va) {
		std::ostringstream oss;
		for (auto v : va) {
			if (v.is<std::string>()) {
				oss << v.as<std::string>();
			}
			else if (v.is<double>()) {
				oss << v.as<double>();
			}
			else if (v.is<int>()) {
				oss << v.as<int>();
			}
			else if (v.is<bool>()) {
				oss << (v.as<bool>() ? "true" : "false");
			}
			else if (v.is<std::nullptr_t>()) {
				oss << "nil";
			}
			else {
				oss << "[unknown type]";
			}
			oss << " ";
		}
		std::string message = oss.str();
		if (!message.empty()) {
			message.pop_back();  // Remove trailing space
		}
		return message;
		};

	lua["print"] = [argsToString](sol::variadic_args va) {
		std::string message = argsToString(va);
		EngineEvents::LuaMessageEvent.Fire(message, LuaMessageType::Info);
	};

	lua["warn"] = [argsToString](sol::variadic_args va) {
		std::string message = argsToString(va);
		EngineEvents::LuaMessageEvent.Fire("WARNING: " + message, LuaMessageType::Warning);
	};

	lua["error"] = [argsToString](sol::variadic_args va) {
		std::string message = argsToString(va);
		EngineEvents::LuaMessageEvent.Fire("ERROR: " + message, LuaMessageType::Error);
		// Note: In Lua, error() typically stops execution. You might want to add that behavior here.
		throw sol::error(message);
	};

	doBindings();

	std::string scriptPath = "./Game/?.lua";
	lua["package"]["path"] = scriptPath;

	try {
		lua.script("require('main')");
	}
	catch (const sol::error& e) {
		EngineEvents::LuaMessageEvent.Fire(e.what(), LuaMessageType::Info);
	}
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