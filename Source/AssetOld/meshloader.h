#pragma once
#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "Asset/assetloader.h"
#include "Asset/loadoptions.h"
#include "Graphic/Resources/mesh.h"

#include <iostream>
#include <string>
#include <memory>

struct MeshLoadOptions {

};

class MeshLoader : public AssetLoader<Mesh, MeshLoadOptions> {
public:
	//~MeshLoader() override {}

    std::shared_ptr<AssetBase>  load(const std::string& name, const MeshLoadOptions& params) override {
        auto mesh = std::make_shared<Mesh>(/* parameters */);
        return std::make_shared<Asset<Mesh>>(name, mesh);
    }
};

#endif 
