#pragma once
#ifndef SOURCES_H
#define SOURCES_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string>
#include <variant>
#include <vector>

struct FileSource {
    std::string filepath;
};


struct CubemapFileSource {
    std::vector<std::string> filepaths;
};

struct MemorySource {
    const unsigned char* data;
    size_t size;
};

struct AiMaterialSource {
    aiMaterial* aiMaterial = nullptr;
    const aiScene* scene = nullptr;
    std::string directory;
};

using AssetSource = std::variant<FileSource, MemorySource, CubemapFileSource, AiMaterialSource>;

#endif 
