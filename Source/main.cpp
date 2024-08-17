#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS

#include "Graphic/graphicscontext.h"
#include "Graphic/renderer.h"
#include "Editor/editor.h"
#include "Script/luaenvironment.h"

#include <spdlog/spdlog.h>
#include <imgui_impl_sdl2.h>

#include <iostream>

int main(int argc, char* argv[]) {
	GraphicsContext graphics;
	if (!graphics.initialize("Roblox Studio", 1280, 720)) {
		std::cerr << "Failed to iniitalize graphics context" << std::endl;
		return -1;
	}

	Renderer renderer(1280, 720, graphics);
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

		// render scene to a framebuffer
		renderer.setFramebuffer(renderer.getDefaultFramebuffer());
		renderer.beginFrame();
		renderer.render();
		renderer.endFrame();

		renderer.renderToScreen();

		//editor.render();

		graphics.swapBuffers();
	}

	return 0;
}

// do post-processing
//renderer.setFramebuffer(&postProcessFramebuffer);
//renderer.beginFrame();
// apply post-processing effects here
//renderer.endFrame();

// render the final result to the screen
//renderer.setFramebuffer(nullptr);
//renderer.renderToScreen();