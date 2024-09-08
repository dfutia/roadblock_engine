#include "Scene/scene.h"
#include "Scene/SceneGraph/instance.h"
#include "Graphic/shader.h"
#include "Graphic/camera.h"
#include "Input/keyboard.h"
#include "Input/mouse.h"

Scene::Scene(Keyboard& keyboard, Mouse& mouse) : 
	keyboard(keyboard), mouse(mouse),
	shader(std::make_unique<Shader>("Asset/Shaders/basic.vert", "Asset/Shaders/basic.frag")),
	camera(std::make_unique<Camera>())
{}

void Scene::update(double deltaTime) {
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
