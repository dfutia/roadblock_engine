#pragma once
#ifndef SCENE_H
#define SCENE_H

#include "Scene/SceneGraph/instance.h"
#include "Graphic/camera.h"
#include "Graphic/shader.h"

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

	Shader* getShader() { return shader.get(); }
	Camera* getCamera() { return camera.get(); }
	std::vector<std::unique_ptr<Instance>>& getInstances() { return instances; }
private:
	Keyboard& keyboard;
	Mouse& mouse;

	std::unique_ptr<Camera> camera;
	std::unique_ptr<Shader> shader;
	std::vector<std::unique_ptr<Instance>> instances;
};

#endif 
