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