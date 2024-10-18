#pragma once
#ifndef BASE_PART_H
#define BASE_PART_H

#include "Scene/Nodes/instance.h"
#include "Graphic/Resources/mesh.h"

#include <glm/glm.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/euler_angles.hpp>

class BasePart : public Instance {
public:
	BasePart(const std::string& name, Mesh mesh) : Instance(name), m_mesh(mesh) {}
	BasePart(const std::string& name) : Instance(name) {}

	virtual ~BasePart() = default;

	std::string getTypeName() const override {
		return "BasePart";
	}

	glm::mat4 getTransform() {
		return glm::translate(glm::mat4(1.0f), m_position)
			* glm::eulerAngleXYZ(glm::radians(m_rotation.x), glm::radians(m_rotation.y), glm::radians(m_rotation.z))
			* glm::scale(glm::mat4(1.0f), m_scale);
	}

	Mesh getMesh() { return m_mesh; }
	void setMesh(Mesh mesh) { m_mesh = mesh; }

	glm::vec4 getColor() const { return m_color; }
	void setColor(const glm::vec4& color) { m_color = color; }

	glm::vec3 getPosition() const { return m_position; }
	void setPosition(const glm::vec3& position) { m_position = position; }

	glm::vec3 getRotation() const { return m_rotation; }
	void setRotation(const glm::vec3& rotation) { m_rotation = rotation; }

	glm::vec3 getScale() const { return m_scale; }
	void setScale(const glm::vec3& scale) { m_scale = scale; }

	std::string getMaterialName() const {
		return m_mesh.material ? m_mesh.material->name : "";
	}

	Material* getMaterial() const {
		return m_mesh.material;
	}

	void setMaterial(const std::string& materialName) {
		//m_mesh.material = MaterialManager::getInstance().getMaterial(materialName).get();
		//if (!m_mesh.material) {
		//	m_mesh.material = MaterialManager::getInstance().createMaterial(materialName).get();
		//}
	}
private:
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);

	glm::vec4 m_color = glm::vec4(1.0f);

	Mesh m_mesh;
};

#endif 