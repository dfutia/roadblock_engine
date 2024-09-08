#pragma once
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "Event/event.h"
#include "Input/inputobject.h"

#include <sol/sol.hpp>

#include <iostream>
#include <unordered_map>

class Keyboard {
public:
	Event<const InputObject&> inputBegan;
	Event<const InputObject&> inputEnded;

	void update() {
		previousKeyStates = currentKeyStates;
	}

	bool isKeyDown(SDL_Keycode sdlKeycode) { 
		Keycode keycode = sdlToKeycode(sdlKeycode);
		return currentKeyStates[keycode];
	}

	bool isKeyPressed(SDL_Keycode sdlKeycode) {
		Keycode keycode = sdlToKeycode(sdlKeycode);
		return currentKeyStates[keycode] && !previousKeyStates[keycode];
	}

	bool isKeyReleased(SDL_Keycode sdlKeycode) {
		Keycode keycode = sdlToKeycode(sdlKeycode);
		return !currentKeyStates[keycode] && previousKeyStates[keycode];
	}

	void onKeyDown(SDL_Event& event) {
		Keycode keycode = sdlToKeycode(event.key.keysym.sym);

		if (currentKeyStates[keycode] == true) { return; }

		//previousKeyStates[keycode] = currentKeyStates[keycode];
		currentKeyStates[keycode] = true;

		InputObject io(keycode);
		inputBegan.Fire(io);
	}

	void onKeyUp(SDL_Event& event) {
		Keycode keycode = sdlToKeycode(event.key.keysym.sym);

		if (!currentKeyStates[keycode]) { return; }

		//previousKeyStates[keycode] = currentKeyStates[keycode];
		currentKeyStates[keycode] = false;

		InputObject io(keycode);
		inputEnded.Fire(io);
	}
private:
	std::unordered_map<Keycode, bool> currentKeyStates;
	std::unordered_map<Keycode, bool> previousKeyStates;

	Keycode sdlToKeycode(SDL_Keycode keycode) {
		switch (keycode) {
		case SDLK_a: return Keycode::A;
		case SDLK_b: return Keycode::B;
		case SDLK_c: return Keycode::C;
		case SDLK_d: return Keycode::D;
		case SDLK_e: return Keycode::E;
		case SDLK_f: return Keycode::F;
		case SDLK_g: return Keycode::G;
		case SDLK_h: return Keycode::H;
		case SDLK_i: return Keycode::I;
		case SDLK_j: return Keycode::J;
		case SDLK_k: return Keycode::K;
		case SDLK_l: return Keycode::L;
		case SDLK_m: return Keycode::M;
		case SDLK_n: return Keycode::N;
		case SDLK_o: return Keycode::O;
		case SDLK_p: return Keycode::P;
		case SDLK_q: return Keycode::Q;
		case SDLK_r: return Keycode::R;
		case SDLK_s: return Keycode::S;
		case SDLK_t: return Keycode::T;
		case SDLK_u: return Keycode::U;
		case SDLK_v: return Keycode::V;
		case SDLK_w: return Keycode::W;
		case SDLK_x: return Keycode::X;
		case SDLK_y: return Keycode::Y;
		case SDLK_z: return Keycode::Z;
		case SDLK_SPACE: return Keycode::Space;
		default: return Keycode::Unknown;
		}
	}
};

#endif 