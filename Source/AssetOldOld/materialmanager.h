#pragma once
#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

#include "Graphic/Resources/material.h"

#include <string>
#include <unordered_map>
#include <memory>

class MaterialManager {
public:
    static MaterialManager& getInstance() {
        static MaterialManager instance;
        return instance;
    }

    std::shared_ptr<Material> createMaterial(const std::string& name);
    std::shared_ptr<Material> getMaterial(const std::string& name);
    void removeMaterial(const std::string& name);
    const std::unordered_map<std::string, std::shared_ptr<Material>>& getAllMaterials() const;
private:
    MaterialManager() {}
    std::unordered_map<std::string, std::shared_ptr<Material>> m_materials;
};

#endif 