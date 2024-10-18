#pragma once
#ifndef PART_H
#define PART_H

#include "Scene/Nodes/basepart.h"
#include "Graphic/Resources/mesh.h"

#include <gtc/quaternion.hpp>
#include <gtx/euler_angles.hpp>

class Part : public BasePart {
public:
	static void createBindings(sol::state& lua) {
		lua.new_usertype<Part>("Part",
			"new", sol::factories([]() { return new Part(); }),
			sol::base_classes, sol::bases<Instance>()
		);
	}

	Part() : BasePart("Part", createCubeMesh(1.0f)) {}

	std::string getTypeName() const override {
		return "Part";
	}
};

#endif 
