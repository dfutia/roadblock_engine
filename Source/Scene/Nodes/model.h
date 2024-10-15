#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "Scene/Nodes/instance.h"

class Model : public Instance {
public:
	static void createBindings(sol::state& lua) {
		lua.new_usertype<Model>("Model",
			"new", sol::factories([]() { return new Model(); }),
			sol::base_classes, sol::bases<Instance>()
		);
	}

	Model() : Instance("Model") {}

	std::string getTypeName() const override {
		return "Model";
	}
};

#endif 