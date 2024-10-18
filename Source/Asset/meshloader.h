#pragma once
#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "Asset/assetloader.h"
#include "Asset/assetmanager.h"
#include "Asset/sources.h"
#include "Graphic/Resources/mesh.h"

#include <iostream>
#include <string>
#include <memory>
#include <vector>

class AssetManager;

class MeshLoader : public AssetLoader<Mesh> {
public:
    std::unique_ptr<MeshHandle> Load(const AssetSource& source, AssetManager& assetManager) override {
        if (std::holds_alternative<FileSource>(source)) {
            return LoadFromFile(std::get<FileSource>(source).filepath);
        }

        return nullptr;
    }

private:
    std::unique_ptr<MeshHandle> LoadFromFile(const std::string& filepath) {
        std::cout << "loading mesh from file" << std::endl;
        return nullptr;
    }
};

#endif
