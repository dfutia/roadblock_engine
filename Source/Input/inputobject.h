#pragma once
#ifndef INPUT_OBJECT_H
#define INPUT_OBJECT_H

#include <string>

#include <SDL2/sdl.h>

enum class Keycode {
	A, B, C, 
	D, E, F,
	G, H, I,
	J, K, L,
	M, N, O,
	P, Q, R,
	S, T, U,
	V, W, X,
	Y, Z,
	Space,
	Unknown,
};

enum class MouseButton {
	Left,
	Right,
	Middle
};

struct InputObject {
	Keycode keycode;
	//MouseButton mouseButton;
	//float X;
	//float Y;
	//std::string UserInputType;

	InputObject(Keycode kc = Keycode::Unknown) : 
		keycode(kc)//, 
		//mouseButton(mb), 
		//X(x), Y(y), 
		//UserInputType(type) 
	{}
};

#endif 