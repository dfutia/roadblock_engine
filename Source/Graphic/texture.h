#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <array>

class Texture {
public:
	GLuint id;
	int width, height, channels;
    bool isCubemap;

    // load from file
    Texture(const std::string& path);
    // load from memory
    Texture(const unsigned char* buffer, int bufferSize);
    Texture(const std::array<std::string, 6>& faces);

    void bind(unsigned int unit = 0);
};

struct TextureStore {
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};

std::shared_ptr<Texture> loadTextureFromFile(const std::string& name, const std::string& path);
std::shared_ptr<Texture> loadTextureFromMemory(const std::string& name, const unsigned char* buffer, int size);
std::shared_ptr<Texture> loadCubemap(const std::string& name, const std::array<std::string, 6>& faces);

extern TextureStore gTextureStore;

#endif 