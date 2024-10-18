#pragma once
#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "Asset/assetloader.h"
#include "Asset/loadoptions.h"
#include "Graphic/Resources/texture.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <iostream>
#include <string>
#include <memory>
#include <array>

struct TextureLoadOptions {
	std::string filepath;
	const aiTexture* embeddedTexture = nullptr;
	bool isCubemap = false;
	std::array<std::string, 6> cubemapPaths;
};

class TextureLoader : public AssetLoader<Texture, TextureLoadOptions> {
public:
	//~TextureLoader() override {}

	std::shared_ptr<AssetBase> load(const std::string& name, const TextureLoadOptions& options) override {
		//if (options.embeddedTexture) {
		//	return loadFromMemory(options.embeddedTexture);
		//}
		//else if (!options.filepath.empty()) {
		//	return loadFromFile(options.filepath);
		//}
		//return nullptr;
		auto texture = std::make_shared<Texture>(/* parameters */);
		return std::make_shared<Asset<Texture>>(name, texture);
	}
private:
	std::shared_ptr<Texture> loadFromMemory(const aiTexture* embeddedTexture) {
		// Implement loading from embedded texture
		return nullptr;
	}

	std::shared_ptr<Texture> loadFromFile(const std::string& filePath) {
		// Implement loading from file
		return nullptr;
	}
};

#endif 
