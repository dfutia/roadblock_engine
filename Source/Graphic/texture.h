#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb_image.h>
#include <glad/glad.h>

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>

class Texture {
public:
	GLuint id;
	int width, height, channels;

    // load from file
	Texture(const std::string& path) {
        std::cout << "loading texture from file" << std::endl;

		//stbi_set_flip_vertically_on_load(true);
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

        if (data) {
            GLenum format;
            if (channels == 1)
                format = GL_RED;
            else if (channels == 3)
                format = GL_RGB;
            else if (channels == 4)
                format = GL_RGBA;

            glGenTextures(1, &id);
            glBindTexture(GL_TEXTURE_2D, id);
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            // Texture Wrapping Parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            // Texture Filtering Parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else {
            std::cout << "failed to load texture from path" << std::endl;
        }
	}

    // load from memory
    Texture(const unsigned char* buffer, int bufferSize) {
        std::cout << "loading texture from memory" << std::endl;
    }
	
	void bind(unsigned int unit = 0) {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, id);
    }
};

struct TextureStore {
    std::unordered_map<std::string, std::shared_ptr<Texture>> textures;
};
TextureStore gTextureStore;

std::shared_ptr<Texture> loadTextureFromFile(const std::string& name, const std::string& path) {
    auto texture = std::make_shared<Texture>(path);
    gTextureStore.textures[name] = texture;
    return texture;
}

std::shared_ptr<Texture> loadTextureFromMemory(const std::string& name, const unsigned char* buffer, int size) {
    auto texture = std::make_shared<Texture>(buffer, size);
    gTextureStore.textures[name] = texture;
    return texture;
}

#endif 