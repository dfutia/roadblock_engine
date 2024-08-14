#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING

#include "graphicscontext.h"
#include "Editor/editor.h"
#include "luaenvironment.h"

#include <spdlog/spdlog.h>
#include <imgui_impl_sdl2.h>

#include <iostream>

int main(int argc, char* argv[]) {
	GraphicsContext graphics;
	if (!graphics.initialize("Roblox Studio", 1280, 720)) {
		std::cerr << "Failed to iniitalize graphics context" << std::endl;
		return -1;
	}

	Editor editor(graphics);
	LuaEnvironment luaEnvironment;

	bool running = true;
	while (running) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			ImGui_ImplSDL2_ProcessEvent(&event);

			switch (event.type) {
			case SDL_QUIT:
				running = false;
				break;
			case SDL_WINDOWEVENT:
				if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
					running = false;
				}
				break;
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);

		editor.render();

		graphics.swapBuffers();
	}

	return 0;
}