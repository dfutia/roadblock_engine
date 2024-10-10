#pragma once
#ifndef INSTANCE_H
#define INSTANCE_H

#include "Reflection/reflectable.h"

#include <sol/sol.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <vector>

class Instance : public Reflectable {
public:
	static void createBindings(sol::state& lua);

	Instance();
	virtual ~Instance();

	std::string getTypeName() const override;

	void setParent(Instance* parent);
	std::vector<Instance*> getChildren();
	std::vector<Instance*> getDescendants();
	glm::mat4 getTransform();

	std::string name;
	Instance* parent;
	std::vector<Instance*> children;
	glm::mat4 transform;
};

#endif 
