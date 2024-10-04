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

#endif 