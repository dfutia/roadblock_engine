#pragma once
#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <fstream>

class Serializable {
public:
	virtual ~Serializable() = default;
	virtual void serialize(std::ostream& out) const = 0;
	virtual void deserialize(std::istream& in) = 0;
};

#endif 
