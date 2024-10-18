#pragma once
#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include "Asset/assetloader.h"
#include "Asset/assetmanager.h"
#include "Asset/sources.h"
#include "Graphic/Resources/texture.h"

#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <memory>
#include <vector>

class AssetManager;

class TextureLoader : public AssetLoader<Texture> {
public:
    std::unique_ptr<TextureHandle> Load(const AssetSource& source, AssetManager& assetManager) override {
        if (std::holds_alternative<FileSource>(source)) {
            return LoadFromFile(std::get<FileSource>(source).filepath);
        }
        else if (std::holds_alternative<MemorySource>(source)) {
            //const auto& memorySource = std::get<MemorySource>(source);
            //return LoadFromMemory(memorySource.data, memorySource.size);
            return nullptr;
        }
        else if (std::holds_alternative<CubemapFileSource>(source)) {
            return LoadCubemap(std::get<CubemapFileSource>(source).filepaths);
        }

        return nullptr;
    }

private:
    std::unique_ptr<TextureHandle> LoadFromFile(const std::string& filepath) {
        std::cout << "Loading texture from file: " << filepath << std::endl;
        stbi_set_flip_vertically_on_load(true);
        int width, height, channels;
        unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);
        if (data) {
            unsigned int id = CreateGLTexture(data, width, height, channels, false);
            stbi_image_free(data);
            return std::make_unique<TextureHandle>(new Texture(id, width, height, channels, false));
        }
        std::cout << "Failed to load texture from path: " << filepath << std::endl;
        return nullptr;
    }

    //std::unique_ptr<TextureHandle> LoadFromMemory(const unsigned char* buffer, size_t bufferSize) {
    //    std::cout << "Loading texture from memory" << std::endl;
    //    int width, height, channels;
    //    unsigned char* data = stbi_load_from_memory(buffer, bufferSize, &width, &height, &channels, 0);
    //    if (data) {
    //        unsigned int id = CreateGLTexture(data, width, height, channels, false);
    //        stbi_image_free(data);
    //        return std::make_unique<TextureHandle>(new Texture(id, width, height, channels, false));
    //    }
    //    std::cout << "Failed to load texture from memory" << std::endl;
    //    return nullptr;
    //}

    std::unique_ptr<TextureHandle> LoadCubemap(std::vector<std::string> filepaths) {
        std::cout << "Loading cubemap" << std::endl;
        if (filepaths.size() != 6) {
            std::cout << "Cubemap requires exactly 6 filepaths" << std::endl;
            return nullptr;
        }

        unsigned int id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_CUBE_MAP, id);

        int width, height, channels;
        for (unsigned int i = 0; i < filepaths.size(); i++) {
            unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &channels, 0);
            if (data) {
                GLenum format = (channels == 3) ? GL_RGB : GL_RGBA;
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                stbi_image_free(data);
            }
            else {
                std::cout << "Cubemap texture failed to load at path: " << filepaths[i] << std::endl;
                stbi_image_free(data);
                glDeleteTextures(1, &id);
                return nullptr;
            }
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return std::make_unique<TextureHandle>(new Texture(id, width, height, channels, true));
    }

    unsigned int CreateGLTexture(unsigned char* data, int width, int height, int channels, bool isCubemap) {
        unsigned int id;
        glGenTextures(1, &id);
        GLenum target = isCubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
        glBindTexture(target, id);

        GLenum format;
        if (channels == 1)
            format = GL_RED;
        else if (channels == 3)
            format = GL_RGB;
        else if (channels == 4)
            format = GL_RGBA;

        glTexImage2D(target, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(target);

        if (!isCubemap) {
            glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        return id;
    }
};

#endif 