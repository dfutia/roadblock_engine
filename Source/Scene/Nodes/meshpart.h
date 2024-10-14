#pragma once
#ifndef MESH_PART_H
#define MESH_PART_H

#include "Scene/Nodes/instance.h"
#include "Graphic/Resources/mesh.h"
#include "Asset/materialmanager.h"

class MeshPart : public Instance {
public:
	//static void createBindings(sol::state& lua) {
	//	lua.new_usertype<Part>("Part",
	//		"new", sol::factories([]() { return new Part(); }),
	//		sol::base_classes, sol::bases<Instance>()
	//	);
	//}

	MeshPart() {
		name = "MeshPart";
		transform = glm::mat4(1.0f);
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		//this->mesh = &createCubeMesh(1.0f);
	}

	MeshPart(Mesh* mesh) {
		name = "MeshPart";
		transform = glm::mat4(1.0f);
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->mesh = mesh;
	}

	std::string getTypeName() const override {
		return "MeshPart";
	}

	Mesh& getMesh() { return *mesh; }
	void setMesh(Mesh* newMesh) { mesh = newMesh; }

	glm::vec4& getColor() { return color; }
	void setColor(const glm::vec4& newColor) { color = newColor; }

	std::string getMaterialName() const {
		return mesh->material ? mesh->material->name : "";
	}

	Material* getMaterial() const {
		return mesh->material;
	}

	void setMaterial(const std::string& materialName) {
		mesh->material = MaterialManager::getInstance().getMaterial(materialName).get();
		if (!mesh->material) {
			mesh->material = MaterialManager::getInstance().createMaterial(materialName).get();
		}
	}
private:
	Mesh* mesh;
	glm::vec4 color;
};

#endif 
