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
    std::shared_ptr<TypedAssetHandle<Material>> Load(const AssetSource& source, AssetManager& assetManager) override {
        if (std::holds_alternative<AiMaterialSource>(source)) {
            const auto& aiMaterialSource = std::get<AiMaterialSource>(source);
            return LoadFromAiMaterial(aiMaterialSource.aiMaterial, aiMaterialSource.scene, aiMaterialSource.directory);
        }
        else if (std::holds_alternative<MemorySource>(source)) {
            const auto& memorySource = std::get<MemorySource>(source);
            if (memorySource.data == nullptr && memorySource.size == 0) {
                auto defaultMaterial = std::make_shared<Material>();
                defaultMaterial->ambient = glm::vec3(0.1f, 0.1f, 0.1f);
                defaultMaterial->diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
                defaultMaterial->specular = glm::vec3(0.5f, 0.5f, 0.5f);
                defaultMaterial->shininess = 32.0f;
                return std::make_shared<MaterialHandle>(std::move(defaultMaterial));
            }
        }
        return nullptr;
    }
private:
    std::shared_ptr<MaterialHandle>  LoadFromAiMaterial(aiMaterial* mat, const aiScene* scene, const std::string& directory) {
        std::cout << "loading material from aiMaterial" << std::endl;
        if (!mat || !scene) {
            return nullptr;
        }

        std::shared_ptr<Material> material = std::make_shared<Material>();

        aiColor3D color(0.0f, 0.0f, 0.0f);
        float shininess;

        mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
        material->ambient = glm::vec3(color.r, color.g, color.b);

        mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        material->diffuse = glm::vec3(color.r, color.g, color.b);

        mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
        material->specular = glm::vec3(color.r, color.g, color.b);

        mat->Get(AI_MATKEY_SHININESS, shininess);
        material->shininess = shininess;

        loadMaterialTextures(material, mat, aiTextureType_DIFFUSE, "texture_diffuse", scene, directory);
        loadMaterialTextures(material, mat, aiTextureType_SPECULAR, "texture_specular", scene, directory);
        loadMaterialTextures(material, mat, aiTextureType_HEIGHT, "texture_normal", scene, directory);

        return std::make_shared<MaterialHandle>(std::move(material), gFilesystem.getFilename(directory));
    }

    void loadMaterialTextures(std::shared_ptr<Material> material, aiMaterial* mat, aiTextureType type,
        const std::string& typeName, const aiScene* scene, const std::string& directory) {
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string textureKey = std::string(str.C_Str());
            const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());

            std::shared_ptr<TextureHandle> textureHandle;
            if (embeddedTexture) {
                //g_assetManager.GetAsset<Texture>(MemorySorce{

                //    });
            }
            else {
                std::string filepath = directory + '/' + textureKey;
                textureHandle = g_assetManager.GetAsset<Texture>(FileSource{ filepath });
            }

            if (textureHandle) {
                material->textureHandles.push_back(textureHandle);
            }
        }
    }
};

#endif
