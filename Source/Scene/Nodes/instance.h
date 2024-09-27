#pragma once
#ifndef INSTANCE_H
#define INSTANCE_H

#include <sol/sol.hpp>
#include <glm/mat4x4.hpp>

#include <string>
#include <vector>

class Instance {
public:
	std::string name;
	Instance* parent;
	std::vector<Instance*> children;
	glm::mat4 transform;

	static void createBindings(sol::state& lua);

	virtual ~Instance();

	void setParent(Instance* parent);
	std::vector<Instance*> getChildren();
	std::vector<Instance*> getDescendants();
	glm::mat4 getTransform() { return transform; }
};

#endif 
