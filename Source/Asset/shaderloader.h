//#pragma once
//#ifndef SHADER_LOADER_H
//#define SHADER_LOADER_H
//
//#include "Asset/assetloader.h"
//#include "Asset/sources.h"
//#include "Graphic/Resources/shader.h"
//
//#include <iostream>
//#include <string>
//#include <memory>
//#include <vector>
//
//class ShaderLoader : public AssetLoader<Shader> {
//public:
//    std::unique_ptr<Shader> Load(const AssetSource& source, AssetManager& assetManager) override {
//        if (std::holds_alternative<FileSource>(source)) {
//            return LoadFromFile(std::get<FileSource>(source).filepath);
//        }
//
//        return nullptr;
//    }
//
//private:
//    std::unique_ptr<Shader> LoadFromFile(const std::string& filepath) {
//        std::cout << "loading shader from file" << std::endl;
//    }
//};
//
//#endif
