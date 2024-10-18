#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <string>
#include <unordered_map>
#include <memory>

class Texture;

class AssetManager {
public:
    std::shared_ptr<Texture> getTexture(const std::string& key);
    void addTexture(const std::string& key, std::shared_ptr<Texture> texture);
private:
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};

extern AssetManager gAssetManager;

#endif 
