#pragma once
#ifndef PART_H
#define PART_H

#include "Scene/Nodes/instance.h"
#include "Graphic/Resources/mesh.h"
#include "Asset/materialmanager.h"

#include <gtc/quaternion.hpp>
#include <gtx/euler_angles.hpp>

class Part : public Instance {
public:
	static void createBindings(sol::state& lua) {
		lua.new_usertype<Part>("Part",
			"new", sol::factories([]() { return new Part(); }),
			sol::base_classes, sol::bases<Instance>()
		);
	}

	Part() { 
		name = "Part"; 
	}

	std::string getTypeName() const override {
		return "Part";
	}

	std::string getName() const { return name; }
	void setName(const std::string& newName) { name = newName; }

	glm::vec4 getColor() const { return m_color; }
	void setColor(const glm::vec4& color) { m_color = color; }

	glm::vec3 getPosition() const { return m_position; }
	void setPosition(const glm::vec3& position) { m_position = position; updateTransform(); }

	glm::vec3 getRotation() const { return m_rotation; }
	void setRotation(const glm::vec3& rotation) { m_rotation = rotation; updateTransform(); }

	glm::vec3 getScale() const { return m_scale; }
	void setScale(const glm::vec3& scale) { m_scale = scale; updateTransform(); }

	Mesh& getMesh() { return mesh; }

	std::string getMaterialName() const {
		return mesh.material ? mesh.material->name : "";
	}

	Material* getMaterial() const {
		return mesh.material;
	}

	void setMaterial(const std::string& materialName) {
		mesh.material = MaterialManager::getInstance().getMaterial(materialName).get();
		if (!mesh.material) {
			mesh.material = MaterialManager::getInstance().createMaterial(materialName).get();
		}
	}
private:
	Mesh mesh = createCubeMesh(1.0f);
	glm::vec3 m_position = glm::vec3(0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f);
	glm::vec4 m_color = glm::vec4(1.0f);

	void updateTransform() {
		transform = glm::translate(glm::mat4(1.0f), m_position)
			* glm::eulerAngleXYZ(glm::radians(m_rotation.x), glm::radians(m_rotation.y), glm::radians(m_rotation.z))
			* glm::scale(glm::mat4(1.0f), m_scale);
	}
};

#endif 
