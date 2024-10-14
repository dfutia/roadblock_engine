#pragma once
#ifndef PART_H
#define PART_H

#include "Scene/Nodes/instance.h"
#include "Graphic/Resources/mesh.h"
#include "Asset/materialmanager.h"

class Part : public Instance {
public:
	static void createBindings(sol::state& lua) {
		lua.new_usertype<Part>("Part",
			"new", sol::factories([]() { return new Part(); }),
			sol::base_classes, sol::bases<Instance>()
		);
	}

	Part() { 
		name = "Part"; 
		transform = glm::mat4(1.0f);
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	std::string getTypeName() const override {
		return "Part";
	}

	Mesh& getMesh() { return mesh; }

	glm::vec4& getColor() { return color; }
	void setColor(const glm::vec4& newColor) { color = newColor; }

	std::string getMaterialName() const {
		return mesh.material ? mesh.material->name : "";
	}

	Material* getMaterial() const {
		return mesh.material;
	}

	void setMaterial(const std::string& materialName) {
		mesh.material = MaterialManager::getInstance().getMaterial(materialName).get();
		if (!mesh.material) {
			mesh.material = MaterialManager::getInstance().createMaterial(materialName).get();
		}
	}
private:
	Mesh mesh = createCubeMesh(1.0f);
	glm::vec4 color;
};

#endif 
