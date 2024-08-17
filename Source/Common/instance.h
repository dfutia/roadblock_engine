#pragma once
#ifndef INSTANCE_H
#define INSTANCE_H

#include "Common/serializable.h"

#include <string>
#include <vector>

class Instance : Serializable {
public:
	std::string name;
	Instance* parent;
	std::vector<Instance*> children;

	virtual ~Instance();

	virtual void setParent(Instance* parent);

	void serialize(std::ostream& out) const override {}
	void deserialize(std::istream& in) override {}

	std::vector<Instance*> getChildren();
	std::vector<Instance*> getDescendants();
};

#endif 
