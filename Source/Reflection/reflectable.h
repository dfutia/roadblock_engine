#pragma once
#ifndef REFLECTABLE_H
#define REFLECTABLE_H

#include <string>

class Reflectable {
public:
	virtual ~Reflectable() = default;
	virtual std::string getTypeName() const = 0;
};

#endif 