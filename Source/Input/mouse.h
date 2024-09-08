#pragma once
#ifndef MOUSE_H
#define MOUSE_H

#include "Script/luaenvironment.h"

#include <glm.hpp>

#include <unordered_map>

class Mouse {
public:
	static void registerLua(sol::state& lua) {
		lua.new_usertype<Mouse>(
			"Mouse",
			"isButtonDown", &Mouse::isButtonDown,
			"isButtonDown", &Mouse::isButtonPressed,
			"isButtonDown", &Mouse::isButtonReleased,
			"isButtonDown", &Mouse::getPosition,
			"isButtonDown", &Mouse::getDelta
		);
	}

	void update() {}

	glm::ivec2 getPosition() { return { 0,0 }; }
	glm::ivec2 getDelta() { return { 0,0 }; }
	bool isButtonDown() { return false; }
	bool isButtonPressed() { return false; }
	bool isButtonReleased() { return false; }
private:
	std::unordered_map<int, bool> m_currentButtonStates;
	std::unordered_map<int, bool> m_previousButtonStates;
};


#endif 