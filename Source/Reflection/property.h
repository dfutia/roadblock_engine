#pragma once
#ifndef PROPERTY_H
#define PROPERTY_H

#include <iostream>
#include <string>

class Reflectable;

class Property {
public:
	Property(const std::string& name, const std::string& section) : m_name(name), m_section(section) {
		//std::cout << "Creating Property with name: '" << name << "'" << std::endl;
	}
	virtual ~Property() = default;

	virtual const std::string& getTypeName() const = 0;
	virtual void* getValue(const Reflectable* object) const = 0;
	virtual void setValue(Reflectable* object, const void* value) const = 0;

	virtual const std::string& getName() const { return m_name; }
	virtual const std::string& getSection() const { return m_section; }
private:
	std::string m_name;
	std::string m_section;
};

#endif 
