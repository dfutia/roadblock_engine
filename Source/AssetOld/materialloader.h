#pragma once
#ifndef MATERIAL_LOADER_H
#define MATERIAL_LOADER_H

#include "Asset/assetloader.h"
#include "Asset/loadoptions.h"
#include "Asset/assetmanager.h"
#include "Asset/textureloader.h"
#include "Graphic/Resources/material.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>
#include <memory>

struct MaterialLoadOptions {
	aiMaterial* aiMaterial = nullptr;
	const aiScene* scene = nullptr;
	std::string directory;
};

class MaterialLoader : public AssetLoader<Material, MaterialLoadOptions> {
public:
	//~MaterialLoader() override {}

    std::shared_ptr<AssetBase>  load(const std::string& name, const MaterialLoadOptions& options) override {
        if (!options.aiMaterial || !options.scene) {
            return nullptr;
        }

        std::shared_ptr<Material> material = std::make_shared<Material>();

        aiColor3D color(0.0f, 0.0f, 0.0f);
        float shininess;

        options.aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
        material->ambient = glm::vec3(color.r, color.g, color.b);

        options.aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        material->diffuse = glm::vec3(color.r, color.g, color.b);

        options.aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
        material->specular = glm::vec3(color.r, color.g, color.b);

        options.aiMaterial->Get(AI_MATKEY_SHININESS, shininess);
        material->shininess = shininess;

        loadMaterialTextures(material, options.aiMaterial, aiTextureType_DIFFUSE, "texture_diffuse", options.scene, options.directory);
        loadMaterialTextures(material, options.aiMaterial, aiTextureType_SPECULAR, "texture_specular", options.scene, options.directory);
        loadMaterialTextures(material, options.aiMaterial, aiTextureType_HEIGHT, "texture_normal", options.scene, options.directory);

        return std::make_shared<Asset<Material>>(name, material);
    }
private:
    void loadMaterialTextures(std::shared_ptr<Material> material, aiMaterial* mat, aiTextureType type,
        const std::string& typeName, const aiScene* scene, const std::string& directory) {
        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);
            std::string textureKey = std::string(str.C_Str());

            auto textureAsset = g_assetManager.getAsset<Texture>(textureKey);
            if (!textureAsset) {
                TextureLoadOptions options;
                const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());

                if (embeddedTexture) {
                    options.embeddedTexture = embeddedTexture;
                }
                else {
                    options.filepath = directory + '/' + textureKey;
                }

                textureAsset = g_assetManager.loadAsset<Texture, TextureLoadOptions>(textureKey, options);
            }

            if (textureAsset) {
                material->textures.push_back(textureAsset->getResource());
            }
        }
    }
};

#endif 
