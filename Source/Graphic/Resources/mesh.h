#pragma once
#ifndef MESH_H
#define MESH_H

#include "Graphic/Resources/material.h"

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <vector>

struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texCoords;
};

struct Mesh {
	unsigned int vao, vbo, ebo;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::shared_ptr<MaterialHandle> materialHandle;
};

Mesh createCubeMesh(float size = 1.0f);

#endif 
