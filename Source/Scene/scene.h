#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Scene/Nodes/instance.h"
#include "Graphic/Rendering/camera.h"
#include "Graphic/Resources/shader.h"
#include "Graphic/Resources/texture.h"

#include <iostream>
#include <vector>
#include <memory>

class Keyboard;
class Mouse;
class Shader;
class Camera;
class Instance;

class Scene {
public:
	Scene(Keyboard& keyboard, Mouse& mouse);

	void update(double deltaTime);

	void addInstance(std::unique_ptr<Instance> instance);

	Shader* getSceneShader() { return sceneShader.get(); }
	Camera* getCamera() { return camera.get(); }
	std::vector<std::unique_ptr<Instance>>& getInstances() { return instances; }
private:
	Keyboard& keyboard;
	Mouse& mouse;

	std::unique_ptr<Texture> skybox;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Shader> sceneShader;
	std::vector<std::unique_ptr<Instance>> instances;
};

#endif 
