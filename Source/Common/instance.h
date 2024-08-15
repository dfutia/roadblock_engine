#pragma once
#ifndef INSTANCE_H
#define INSTANCE_H

#include <string>
#include <vector>

class Instance {
public:
	std::string name;
	Instance* parent;
	std::vector<Instance*> children;

	virtual ~Instance();

	virtual void setParent(Instance* parent);

	std::vector<Instance*> getChildren();
	std::vector<Instance*> getDescendants();
};

#endif 
