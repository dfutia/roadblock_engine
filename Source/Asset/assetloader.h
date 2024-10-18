#pragma once
#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include "Asset/sources.h"

#include <memory>

class AssetManager;

class BaseAssetLoader {
public:
    virtual ~BaseAssetLoader() = default;
};

template<typename T>
class AssetLoader : public BaseAssetLoader {
public:
    virtual ~AssetLoader() = default;
    virtual std::unique_ptr<TypedAssetHandle<T>> Load(const AssetSource& source, AssetManager& assetManager) = 0;
};

#endif 
