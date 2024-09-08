#pragma once
#ifndef INSTANCE_H
#define INSTANCE_H

#include "Scene/serializable.h"

#include <sol/sol.hpp>

#include <string>
#include <vector>

class Instance {
public:
	std::string name;
	Instance* parent;
	std::vector<Instance*> children;

	virtual ~Instance();

	virtual void setParent(Instance* parent);

	//void serialize(std::ostream& out) const override {}
	//void deserialize(std::istream& in) override {}

	static void createBindings(sol::state& lua);

	std::vector<Instance*> getChildren();
	std::vector<Instance*> getDescendants();
};

#endif 
