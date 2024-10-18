#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
    Texture(unsigned int id, int width, int height, int channels, bool isCubemap)
        : id(id), width(width), height(height), channels(channels), isCubemap(isCubemap) {}

    unsigned int getId() { return id; }
private:
    unsigned int id;
    int width;
    int height;
    int channels;
    bool isCubemap;
};

#endif 