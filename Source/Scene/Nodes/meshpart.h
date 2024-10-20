#pragma once
#ifndef MESH_PART_H
#define MESH_PART_H

#include "Scene/Nodes/basepart.h"
#include "Asset/assetmanager.h"

#include <memory>

class MeshPart : public BasePart {
public:
	//static void createBindings(sol::state& lua) {
	//	lua.new_usertype<Part>("Part",
	//		"new", sol::factories([]() { return new Part(); }),
	//		sol::base_classes, sol::bases<Instance>()
	//	);
	//}

	MeshPart() : BasePart("MeshPart", createCubeMesh(1.0f)) {}
	MeshPart(Mesh mesh) : BasePart("MeshPart", mesh) {}

	std::string getTypeName() const override {
		return "MeshPart";
	}

	std::shared_ptr<MeshHandle> getMeshHandle() const { return m_meshHandle; }
	void setMeshHandle(const std::shared_ptr<MeshHandle>& handle) {
		m_meshHandle = handle;
		updateMesh();
	}
private:
	std::shared_ptr<MeshHandle> m_meshHandle;

	void updateMesh() {
		if (m_meshHandle) {
			setMesh(*m_meshHandle->Get());
		}
	}
};

#endif 
