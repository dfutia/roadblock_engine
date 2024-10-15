#pragma once
#ifndef MESH_PART_H
#define MESH_PART_H

#include "Scene/Nodes/basepart.h"

class MeshPart : public BasePart {
public:
	//static void createBindings(sol::state& lua) {
	//	lua.new_usertype<Part>("Part",
	//		"new", sol::factories([]() { return new Part(); }),
	//		sol::base_classes, sol::bases<Instance>()
	//	);
	//}

	MeshPart() : BasePart("MeshPart", createCubeMesh(1.0f)) {}
	MeshPart(Mesh mesh) : BasePart("MeshPart", mesh) {}

	std::string getTypeName() const override {
		return "MeshPart";
	}
};

#endif 
