#pragma once
#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H



#include <memory>
#include <string>

class Texture;

struct aiTexture;

class TextureLoader {
public:
	static std::shared_ptr<Texture> loadFromFile(const std::string& filepath) {
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(filepath);
		return texture;
	}

	//static std::shared_ptr<Texture> loadFromMemroy(const std::string& filepath) {
	//	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	//	return texture;
	//}
	//static std::shared_ptr<Texture> loadFromMemory(const aiTexture* embeddedTexture) {
	//	std::shared_ptr<Texture> texture = std::make_shared<Texture>();
	//	return texture;
	//}

	static std::shared_ptr<Texture> loadCubemap(const std::string& name, const std::array<std::string, 6>& faces) {
		auto texture = std::make_shared<Texture>(faces);
		return texture;
	}
};

#endif
