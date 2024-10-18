#pragma once
#ifndef SYBOX_H
#define SKYBOX_H

#include "Scene/Nodes/instance.h"
#include "Asset/assetmanager.h"
#include "Asset/textureloader.h"
#include "Graphic/Resources/texture.h"
#include "Graphic/Resources/shader.h"
#include "Graphic/Resources/mesh.h"

#include <array>
#include <memory>

class Skybox : public Instance {
public:
	Skybox() : Instance("Skybox")
	{
		m_texture = g_assetManager.GetAsset<Texture>(CubemapFileSource{ {
			"Asset/Textures/skybox/right.jpg",
			"Asset/Textures/skybox/left.jpg",
			"Asset/Textures/skybox/top.jpg",
			"Asset/Textures/skybox/bottom.jpg",
			"Asset/Textures/skybox/front.jpg",
			"Asset/Textures/skybox/back.jpg"
		} })->Get();
		m_mesh = createCubeMesh(1.0f);
		m_shader = std::make_unique<Shader>("Asset/Shaders/Skybox/skybox.vert", "Asset/Shaders/Skybox/skybox.frag");
	}
	~Skybox() = default;

	std::string getTypeName() const override { return "Skybox"; }

	Shader* getShader() { return m_shader.get(); }
	Mesh& getMesh() { return m_mesh; }
	Texture* getTexture() { return m_texture; }
private:
	std::unique_ptr<Shader> m_shader;
	Texture* m_texture;
	Mesh m_mesh;
};

#endif 
