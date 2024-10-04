#pragma once
#ifndef MESH_PART_H
#define MESH_PART_H

#include "Scene/Nodes/instance.h"
#include "Graphic/Resources/mesh.h"

class MeshPart : public Instance {
public:
	MeshPart() {
		name = "MeshPart";
		transform = glm::mat4(1.0f);
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	MeshPart(Mesh* mesh) {
		name = "MeshPart";
		transform = glm::mat4(1.0f);
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		this->mesh = mesh;
	}

	static void createBindings(sol::state& lua) {
		lua.new_usertype<Part>("Part",
			"new", sol::factories([]() { return new Part(); }),
			sol::base_classes, sol::bases<Instance>()
		);
	}

	Mesh& getMesh() { return *mesh; }
	void setMesh(Mesh* newMesh) { mesh = newMesh; }
	glm::vec4& getColor() { return color; }
	void setColor(const glm::vec4& newColor) { color = newColor; }
private:
	Mesh* mesh;
	glm::vec4 color;
};

#endif 
