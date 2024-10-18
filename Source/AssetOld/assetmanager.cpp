#include "Asset/assetmanager.h"
#include "Asset/textureloader.h"
#include "Asset/meshloader.h"
#include "Asset/materialloader.h"

#include "Graphic/Resources/texture.h"
#include "Graphic/Resources/mesh.h"
#include "Graphic/Resources/material.h"

AssetManager g_assetManager;

AssetManager::AssetManager() {
    registerLoader<Texture, TextureLoadOptions>(std::make_unique<TextureLoader>());
    registerLoader<Mesh, MeshLoadOptions>(std::make_unique<MeshLoader>());
    registerLoader<Material, MaterialLoadOptions>(std::make_unique<MaterialLoader>());
    //registerLoader<Shader>(std::make_unique<ShaderLoader>());
}