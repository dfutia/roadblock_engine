#pragma once
#ifndef MODEL_H
#define MODEL_H

#include "Scene/SceneGraph/instance.h"

class Model : public Instance {
public:
	Model() { name = "Model"; }

	static void createBindings(sol::state& lua) {
		lua.new_usertype<Model>("Model",
			"new", sol::factories([]() { return new Model(); }),
			sol::base_classes, sol::bases<Instance>()
		);
	}
};

#endif 