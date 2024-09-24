#pragma once
#ifndef MOUSE_H
#define MOUSE_H

#include "Script/luaenvironment.h"

#include <SDL2/sdl.h>
#include <glm.hpp>

#include <unordered_map>

class Mouse {
public:
	glm::ivec2 getPosition() const { return position; }
	glm::ivec2 getDelta() const { return delta; }

	bool isButtonDown(int button) const {
		auto it = buttonsDown.find(button);
		return it != buttonsDown.end() && it->second;
	}

	bool isButtonPressed(int button) const {
		auto it = buttonsPressed.find(button);
		return it != buttonsPressed.end() && it->second;
	}

	bool isButtonReleased(int button) const {
		auto it = buttonsReleased.find(button);
		return it != buttonsReleased.end() && it->second;
	}

	void onMouseButtonDown(SDL_Event& event) {
		buttonsDown[event.button.button] = true;
		buttonsPressed[event.button.button] = true;
	}

	void onMouseButtonUp(SDL_Event& event) {
		buttonsDown[event.button.button] = false;
		buttonsReleased[event.button.button] = true;
	}

	void onMouseMotion(SDL_Event& event) {
		delta.x = event.motion.xrel;
		delta.y = event.motion.yrel;
		position.x = event.motion.x;
		position.y = event.motion.y;
	}

	void onMouseWheel(SDL_Event& event) {
		wheelDelta.x = event.wheel.x;
		wheelDelta.y = event.wheel.y;
	}

	void reset() {
		buttonsPressed.clear();
		buttonsReleased.clear();
		delta = { 0, 0 };
		wheelDelta = { 0, 0 };
	}
private:
	glm::ivec2 position = { 0, 0 }; 
	glm::ivec2 delta = { 0, 0 };  
	glm::ivec2 wheelDelta = { 0, 0 };  

	std::unordered_map<int, bool> buttonsDown;
	std::unordered_map<int, bool> buttonsPressed;
	std::unordered_map<int, bool> buttonsReleased;
};


#endif 