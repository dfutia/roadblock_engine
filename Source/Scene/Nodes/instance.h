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

	Instance() = default;
	Instance(const std::string& name);
	virtual ~Instance();

	std::string getTypeName() const override;

	Instance* getParent();
	void setParent(Instance* parent);
	std::vector<Instance*> getChildren();
	std::vector<Instance*> getDescendants();

	std::string getName() const { return m_name; }
	void setName(const std::string& name) { m_name = name; }
private:
	std::string m_name;
	Instance* m_parent;
	std::vector<Instance*> m_children;
};

#endif 
