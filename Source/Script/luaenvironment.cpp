#include "Script/luaenvironment.h"
#include "Event/engineevents.h"
#include "Scene/SceneGraph/instance.h"
#include "Scene/SceneGraph/model.h"
#include "Scene/SceneGraph/part.h"
#include "Scene/SceneGraph/script.h"
#include "Input/keyboard.h"
#include "Input/mouse.h"

LuaEnvironment::LuaEnvironment(Keyboard& kb, Mouse& m) :
	keyboard(kb), mouse(m)
{
	lua.open_libraries(sol::lib::base, sol::lib::package);

	registerLua();

	std::string scriptPath = "./Game/?.lua";
	lua["package"]["path"] = scriptPath;

	try {
		lua.script("require('main')");
	}
	catch (const sol::error& e) {
		EngineEvents::LuaMessageEvent.Fire(e.what(), LuaMessageType::Info);
	}
}

LuaEnvironment::~LuaEnvironment() {
	lua.collect_garbage();
}

void LuaEnvironment::registerLua() {
	registerInput();
}

void LuaEnvironment::registerInput() {
	lua.new_usertype<Keyboard>("Keyboard",
		"isKeyDown", &Keyboard::isKeyDown,
		"isKeyPressed", &Keyboard::isKeyPressed,
		"isKeyReleased", &Keyboard::isKeyReleased,
		"inputBegan", &Keyboard::inputBegan,
		"inputEnded", &Keyboard::inputEnded
	);

	lua.new_usertype<Event<const InputObject&>>(
		"InputEvent",
		"Connect", [](Event<const InputObject&>& event, sol::function callback) {
			return event.Connect([callback](const InputObject& io) {
				callback(io);
				});
		}
	);

	lua.new_usertype<Event<const InputObject&>::ConnectionHandle>(
		"ConnectionHandle",
		"Disconnect", &Event<const InputObject&>::ConnectionHandle::Disconnect
	);

	lua.new_usertype<InputObject>("InputObject",
		"keycode", &InputObject::keycode
	);

	lua.new_enum("Keycode",
		"A", Keycode::A,
		"B", Keycode::B,
		"C", Keycode::C,
		"D", Keycode::D,
		"E", Keycode::E,
		"F", Keycode::F,
		"G", Keycode::G,
		"H", Keycode::H,
		"I", Keycode::I,
		"J", Keycode::J,
		"K", Keycode::K,
		"L", Keycode::L,
		"M", Keycode::M,
		"N", Keycode::N,
		"O", Keycode::O,
		"P", Keycode::P,
		"Q", Keycode::Q,
		"R", Keycode::R,
		"S", Keycode::S,
		"T", Keycode::T,
		"U", Keycode::U,
		"V", Keycode::V,
		"W", Keycode::W,
		"X", Keycode::X,
		"Y", Keycode::Y,
		"Z", Keycode::Z,
		"Space", Keycode::Space,
		"Unknown", Keycode::Unknown
	);

	lua.create_named_table("Input");
	lua["Input"]["Keyboard"] = &keyboard;
	lua["Input"]["Mouse"] = &mouse;
}

//Instance::createBindings(lua);
//Model::createBindings(lua);
//Part::createBindings(lua);
//Mouse::registerLua(lua);
//Keyboard::registerLua(lua);


//lua["input"] = lua.create_table_with(
//	"keyboard", &keyboard
//);

//lua["Instance"] = lua.create_table_with(
//	"new", [](const std::string& className) -> Instance* {
//		if (className == "Model") return new Model();
//		if (className == "Part") return new Part();
//		return nullptr;
//	}
//);

//auto argsToString = [](sol::variadic_args va) {
//	std::ostringstream oss;
//	for (auto v : va) {
//		if (v.is<std::string>()) {
//			oss << v.as<std::string>();
//		}
//		else if (v.is<double>()) {
//			oss << v.as<double>();
//		}
//		else if (v.is<int>()) {
//			oss << v.as<int>();
//		}
//		else if (v.is<bool>()) {
//			oss << (v.as<bool>() ? "true" : "false");
//		}
//		else if (v.is<std::nullptr_t>()) {
//			oss << "nil";
//		}
//		else {
//			oss << "[unknown type]";
//		}
//		oss << " ";
//	}
//	std::string message = oss.str();
//	if (!message.empty()) {
//		message.pop_back();  // Remove trailing space
//	}
//	return message;
//	};
//
//lua["print"] = [argsToString](sol::variadic_args va) {
//	std::string message = argsToString(va);
//	EngineEvents::LuaMessageEvent.Fire(message, LuaMessageType::Info);
//	};
//
//lua["warn"] = [argsToString](sol::variadic_args va) {
//	std::string message = argsToString(va);
//	EngineEvents::LuaMessageEvent.Fire("WARNING: " + message, LuaMessageType::Warning);
//	};
//
//lua["error"] = [argsToString](sol::variadic_args va) {
//	std::string message = argsToString(va);
//	EngineEvents::LuaMessageEvent.Fire("ERROR: " + message, LuaMessageType::Error);
//	// Note: In Lua, error() typically stops execution. You might want to add that behavior here.
//	throw sol::error(message);
//	};