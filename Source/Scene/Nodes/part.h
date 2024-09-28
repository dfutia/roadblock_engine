#pragma once
#ifndef PART_H
#define PART_H

#include "Scene/Nodes/instance.h"
#include "Graphic/Resources/mesh.h"

class Part : public Instance {
public:
	Part() { 
		name = "Part"; 
		transform = glm::mat4(1.0f);
		color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	static void createBindings(sol::state& lua) {
		lua.new_usertype<Part>("Part",
			"new", sol::factories([]() { return new Part(); }),
			sol::base_classes, sol::bases<Instance>()
		);
	}

	Mesh& getMesh() { return mesh; }
	glm::vec4& getColor() { return color; }
	void setColor(const glm::vec4& newColor) { color = newColor; }
private:
	Mesh mesh = createCubeMesh(1.0f);
	glm::vec4 color;
};

#endif 
