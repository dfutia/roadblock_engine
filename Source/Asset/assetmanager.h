#pragma once
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include "Asset/asset.h"
#include "Asset/sources.h"
#include "Asset/assetloader.h"

#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <typeindex>
#include <any>
#include <numeric>

class Texture;
class Material;
class Mesh;

using TextureHandle = TypedAssetHandle<Texture>;
using MaterialHandle = TypedAssetHandle<Material>;
using MeshHandle = TypedAssetHandle<Mesh>;

class AssetManager {
public:
    AssetManager();

    template<typename T>
    void RegisterLoader(std::unique_ptr<AssetLoader<T>> loader) {
        std::cout << "Registering asset loader" << std::endl;
        m_loaders[typeid(T)] = std::move(loader);
    }

    template<typename T>
    std::shared_ptr<TypedAssetHandle<T>> GetAsset(const AssetSource& source, bool isUserAsset = true) {
        std::cout << "Getting asset" << std::endl;
        std::string key = CreateKey(source);
        auto it = m_assets.find(key);
        if (it != m_assets.end()) {
            std::cout << "Asset already exists" << std::endl;
            return std::static_pointer_cast<TypedAssetHandle<T>>(it->second);
        }

        auto loader = GetLoader<T>();
        if (!loader) {
            std::cout << "Getting asset failed. Loader does not exist for this asset type" << std::endl;
            return nullptr;
        }

        std::cout << "Asset does not already exist. Loading asset" << std::endl;

        auto assetHandle = loader->Load(source, *this);
        assetHandle->setUserAsset(isUserAsset);
        m_assets[key] = assetHandle;
        return std::static_pointer_cast<TypedAssetHandle<T>>(assetHandle);
    }

    const std::unordered_map<std::string, std::shared_ptr<AssetHandle>>& getAllAssets() {
        static std::unordered_map<std::string, std::shared_ptr<AssetHandle>> userAssets;
        for (const auto& [key, asset] : m_assets) {
            //std::cout << key << std::endl;
            if (asset->isUserAsset()) {
                //std::cout << key << " is user asset" << std::endl;
                userAssets[key] = asset;
            }
            else {
                //std::cout << key << " not user asset" << std::endl;
            }
        }
        if (userAssets.size() > 1000000) {  // Arbitrary large number
            std::cerr << "Error: Asset count exceeds reasonable limit. Clearing assets." << std::endl;
            userAssets.clear();
        }
        return userAssets;
    }
private:
    std::string CreateKey(const AssetSource& source) {
        if (std::holds_alternative<FileSource>(source)) {
            return std::get<FileSource>(source).filepath;
        }
        else if (std::holds_alternative<MemorySource>(source)) {
            return std::to_string(reinterpret_cast<uintptr_t>(std::get<MemorySource>(source).data));
        }
        else if (std::holds_alternative<CubemapFileSource>(source)) {
            // Join all filepaths into a single string
            const auto& filepaths = std::get<CubemapFileSource>(source).filepaths;
            return std::accumulate(filepaths.begin(), filepaths.end(), std::string(),
                [](const std::string& a, const std::string& b) { return a.empty() ? b : a + ";" + b; });
        }
        else if (std::holds_alternative<AiMaterialSource>(source)) {
            const auto& aiMaterialSource = std::get<AiMaterialSource>(source);
            return aiMaterialSource.directory + "_" + std::to_string(reinterpret_cast<uintptr_t>(aiMaterialSource.aiMaterial));
        }
        else {
            throw std::runtime_error("Unsupported AssetSource type");
        }
    }

    template<typename T>
    AssetLoader<T>* GetLoader() {
        auto it = m_loaders.find(typeid(T));
        if (it != m_loaders.end()) {
            return static_cast<AssetLoader<T>*>(it->second.get());
        }
        return nullptr;
    }

    std::unordered_map<std::string, std::shared_ptr<AssetHandle>> m_assets;
    std::unordered_map<std::type_index, std::unique_ptr<BaseAssetLoader>> m_loaders;
};

extern AssetManager g_assetManager;

#endif