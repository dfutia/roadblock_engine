#include "assetmanager.h"

AssetManager gAssetManager;

std::shared_ptr<Texture> AssetManager::getTexture(const std::string& key) {
    auto it = textures.find(key);
    if (it != textures.end()) {
        return it->second;
    }
    return nullptr;
}

void AssetManager::addTexture(const std::string& key, std::shared_ptr<Texture> texture) {
    textures[key] = texture;
}