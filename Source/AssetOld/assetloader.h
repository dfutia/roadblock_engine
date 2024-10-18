#pragma once
#ifndef ASSET_LOADER_H
#define ASSET_LOADER_H

#include "Asset/asset.h"

#include <string>
#include <memory>

class LoaderBase {
public:
    virtual ~LoaderBase() = default;
};

template<typename T, typename TParams>
class AssetLoader : public LoaderBase {
public:
    virtual std::shared_ptr<AssetBase> load(const std::string& name, const TParams& params) = 0;
};

#endif 
