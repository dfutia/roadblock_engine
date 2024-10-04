#include "Scene/scene.h"
#include "Scene/Nodes/instance.h"
#include "Graphic/Resources/shader.h"
#include "Graphic/Rendering/camera.h"
#include "Input/Devices/keyboard.h"
#include "Input/Devices/mouse.h"

Scene::Scene(Keyboard& keyboard, Mouse& mouse) : 
	keyboard(keyboard), mouse(mouse),
	//sceneShader(std::make_unique<Shader>("Asset/Shaders/basic.vert", "Asset/Shaders/basic.frag")),
	sceneShader(std::make_unique<Shader>("Asset/Shaders/texture.vert", "Asset/Shaders/texture.frag")),
	camera(std::make_unique<Camera>())
{}

void Scene::update(double deltaTime) {
	glm::ivec2 delta = mouse.getDelta();
	if (mouse.isButtonDown(SDL_BUTTON_RIGHT)) { 
		camera->processMouseMovement(static_cast<float>(delta.x), static_cast<float>(delta.y));
	}

	if (keyboard.isKeyDown(SDLK_w)) {
		camera->processKeyboard(FORWARD, deltaTime);
	}

	if (keyboard.isKeyDown(SDLK_s)) {
		camera->processKeyboard(BACKWARD, deltaTime);
	}

	if (keyboard.isKeyDown(SDLK_a)) {
		camera->processKeyboard(LEFT, deltaTime);
	}

	if (keyboard.isKeyDown(SDLK_d)) {
		camera->processKeyboard(RIGHT, deltaTime);
	}
}

void Scene::addInstance(std::unique_ptr<Instance> instance) {
	instances.push_back(std::move(instance));
}
