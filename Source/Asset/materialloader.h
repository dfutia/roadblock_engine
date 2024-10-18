#pragma once
#ifndef MATERIAL_LOADER_H
#define MATERIAL_LOADER_H

#include "Asset/assetloader.h"
#include "Asset/assetmanager.h"
#include "Asset/sources.h"
#include "Graphic/Resources/material.h"

#include <iostream>
#include <string>
#include <memory>
#include <vector>

class AssetManager;

class MaterialLoader : public AssetLoader<Material> {
public:
    std::unique_ptr<MaterialHandle> Load(const AssetSource& source, AssetManager& assetManager) override {
        if (std::holds_alternative<AiMaterialSource>(source)) {
            const auto& aiMaterialSource = std::get<AiMaterialSource>(source);
            return LoadFromAiMaterial(aiMaterialSource.aiMaterial, aiMaterialSource.scene, aiMaterialSource.directory);
        }

        return nullptr;
    }

private:
    std::unique_ptr<MaterialHandle>  LoadFromAiMaterial(aiMaterial* material, const aiScene* scene, const std::string& directory) {
        std::cout << "loading material from file" << std::endl;
        return nullptr;
    }

    void loadMaterialTextures() {

    }
};

#endif
