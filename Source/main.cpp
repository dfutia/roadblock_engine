#define _SILENCE_STDEXT_ARR_ITERS_DEPRECATION_WARNING
#define _CRT_SECURE_NO_WARNINGS
#define _SILENCE_ALL_MS_EXT_DEPRECATION_WARNINGS

#include "Reflection/typeregistry.h"
#include "Graphic/graphicscontext.h"
#include "Graphic/Rendering/renderer.h"
#include "Scene/scene.h"
#include "Audio/audio.h"
#include "Editor/editor.h"
#include "Script/luaenvironment.h"
#include "Input/Devices/keyboard.h"
#include "Input/Devices/mouse.h"
#include "filesystem.h"

#include <spdlog/spdlog.h>
#include <imgui_impl_sdl2.h>

#include <iostream>

//struct Configuration {
//	std::string title = "Untitled Project";
//	bool fullscreen = false;
//	int width = 1280;
//	int height = 720;
//
//	Configuration(sol::state& lua) {
//		sol::load_result script = lua.load_file("Game/config.lua");
//		if (!script.valid()) {
//			return;
//		}
//		lua.script_file("Game/config.lua");
//		sol::table config = lua["config"];
//		title = config.get_or("title", title);
//		fullscreen = config.get_or("fullscreen", fullscreen);
//		width = config.get_or("resolution.width", width);
//		height = config.get_or("resolution.height", height);
//	}
//};

//sol::state lua;
//lua.open_libraries(sol::lib::base, sol::lib::package);
//Configuration config(lua);

int main(int argc, char* argv[]) {
	REGISTER_TYPE(Part);
	REGISTER_PROPERTY(Part, Name, "Data", getName, setName);
	REGISTER_PROPERTY(Part, Position, "Transform", getPosition, setPosition);
	REGISTER_PROPERTY(Part, Rotation, "Transform", getRotation, setRotation);
	REGISTER_PROPERTY(Part, Scale, "Transform", getScale, setScale);
	REGISTER_PROPERTY(Part, Color, "Appearance", getColor, setColor);

	REGISTER_TYPE(Script);
	REGISTER_PROPERTY(Script, Name, "Data", getName, setName);

	REGISTER_TYPE(Model);

	REGISTER_TYPE(Skybox);

	REGISTER_TYPE(MeshPart);
	REGISTER_PROPERTY(MeshPart, Name, "Data", getName, setName);
	REGISTER_PROPERTY(MeshPart, Position, "Transform", getPosition, setPosition);
	REGISTER_PROPERTY(MeshPart, Rotation, "Transform", getRotation, setRotation);
	REGISTER_PROPERTY(MeshPart, Scale, "Transform", getScale, setScale);
	REGISTER_PROPERTY(MeshPart, Color, "Appearance", getColor, setColor);
	REGISTER_PROPERTY(MeshPart, MeshHandle, "Appearance", getMeshHandle, setMeshHandle);

	GraphicsContext graphics;
	if (!graphics.initialize("Roadblock Studio", 1280, 720)) {
		std::cerr << "Failed to iniitalize graphics context" << std::endl;
		return -1;
	}

	Keyboard keyboard;
	Mouse mouse;
	Scene scene(keyboard, mouse);
	Renderer renderer(1280, 720, graphics);
	Audio audio;
	Editor editor(graphics, renderer, scene, audio);
	LuaEnvironment luaEnvironment(keyboard, mouse);

	bool running = true;

	const int FPS = 60;
	const int MS_PER_FRAME = 1000 / FPS;
	const double FIXED_UPDATE_STEP = 1.0 / 60.0;

	double accumulatedTime = 0.0;
	int msPreviousFrame = 0;

	while (running) {
#pragma region Process Input
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
				} else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
				}
				break;
			case SDL_KEYDOWN:
				//if (event.key.repeat == 0) {
				if (event.key.keysym.sym == SDLK_F1) {
					std::cout << "play mode" << std::endl;
				}
				//}
				keyboard.onKeyDown(event);
				break;
			case SDL_KEYUP:
				keyboard.onKeyUp(event);
				break;
			case SDL_MOUSEMOTION:
				mouse.onMouseMotion(event);
				break;
			case SDL_MOUSEBUTTONDOWN:
				mouse.onMouseButtonDown(event);
				break;
			case SDL_MOUSEBUTTONUP:
				mouse.onMouseButtonUp(event);
				break;
			case SDL_MOUSEWHEEL:
				mouse.onMouseWheel(event);
				break;
			case SDL_DROPFILE:
				editor.onFileDrop(event);
				break;
			}
		}
#pragma endregion

#pragma region Update
		// cap the frame rate
		//int timeToWait = MS_PER_FRAME - (SDL_GetTicks() - msPreviousFrame);
		//if (timeToWait > 0 && timeToWait <= MS_PER_FRAME) {
		//	SDL_Delay(timeToWait);
		//}

		// calculate delta time
		int msCurrentFrame = SDL_GetTicks();
		double deltaTime = (msCurrentFrame - msPreviousFrame) / 1000.0;
		msPreviousFrame = msCurrentFrame;

#pragma region Fixed Update
		accumulatedTime += deltaTime;
		while (accumulatedTime >= FIXED_UPDATE_STEP) {
			accumulatedTime -= FIXED_UPDATE_STEP;
		}
#pragma endregion

		editor.update(scene);
		scene.update(deltaTime);
		keyboard.update();
		mouse.reset();
#pragma endregion

#pragma region Render
		renderer.beginFrame();
		renderer.render(scene);
		renderer.endFrame();

		editor.render(scene);

		graphics.swapBuffers();
#pragma endregion
	}

	return 0;
}