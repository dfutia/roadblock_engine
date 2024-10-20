#include "Asset/assetmanager.h"
#include "Asset/textureloader.h"
#include "Asset/meshloader.h"
#include "Asset/materialloader.h"
#include "Graphic/Resources/mesh.h"
#include "Graphic/Resources/texture.h"
#include "Graphic/Resources/material.h"

AssetManager g_assetManager;

AssetManager::AssetManager() {
    RegisterLoader<Texture>(std::make_unique<TextureLoader>());
    RegisterLoader<Mesh>(std::make_unique<MeshLoader>());
    RegisterLoader<Material>(std::make_unique<MaterialLoader>());
}

void AssetManager::init() {
    //Mesh defaultCubeMesh = createCubeMesh(1.0f);
    //g_assetManager.GetAsset<Mesh>(MemorySource{
    //    reinterpret_cast<const unsigned char*>(&defaultCubeMesh),
    //    sizeof(Mesh)
    //    });

    //auto defaultMaterial = std::make_shared<Material>();
    //defaultMaterial->name = "DefaultMaterial";
    //defaultMaterial->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    //defaultMaterial->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    //defaultMaterial->specular = glm::vec3(0.5f, 0.5f, 0.5f);
    //defaultMaterial->shininess = 32.0f;
    //g_assetManager.GetAsset<Material>(MemorySource{
    //    reinterpret_cast<const unsigned char*>(defaultMaterial.get()),
    //    sizeof(Material)
    //    });
}