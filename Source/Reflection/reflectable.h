#pragma once
#ifndef REFLECTABLE_H
#define REFLECTABLE_H

#include "Reflection/typeregistry.h"
#include "Reflection/property.h"

#include <string>
#include <memory>
#include <vector>

class Reflectable {
public:
	virtual ~Reflectable() = default;
	virtual std::string getTypeName() const = 0;
	virtual const std::vector<std::unique_ptr<Property>>& getProperties() const {
		return gTypeRegistry.getProperties(getTypeName());
	}
};

#endif 