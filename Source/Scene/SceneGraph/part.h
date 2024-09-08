#pragma once
#ifndef PART_H
#define PART_H

#include "Scene/SceneGraph/instance.h"
#include "Graphic/mesh.h"

class Part : public Instance {
public:
	Part() { name = "Part"; }

	static void createBindings(sol::state& lua) {
		lua.new_usertype<Part>("Part",
			"new", sol::factories([]() { return new Part(); }),
			sol::base_classes, sol::bases<Instance>()
		);
	}

	Mesh& getMesh() { return mesh; }
private:
	Mesh mesh = createCubeMesh(1.0f);
};

#endif 
