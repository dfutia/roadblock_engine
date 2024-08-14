#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

#include "graphicscontext.h"
#include "luaenvironment.h"
#include "event.h"

#include <spdlog/spdlog.h>

#include <iostream>

int main(int argc, char* argv[]) {
	GraphicsContext graphics;
	LuaEnvironment luaEnvironment;

	if (!graphics.initialize("Roblox Studio", 800, 600)) {
		std::cerr << "Failed to iniitalize graphics context" << std::endl;
		return -1;
	}

	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {
				running = false;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		graphics.swapBuffers();
	}

	return 0;
}