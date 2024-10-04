#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Texture;

struct Material {
	std::string name;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;
	std::vector<std::shared_ptr<Texture>> textures;
};

#endif 