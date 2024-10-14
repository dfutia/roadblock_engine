#include "Asset/materialmanager.h"

std::shared_ptr<Material> MaterialManager::createMaterial(const std::string& name) {
    auto material = std::make_shared<Material>();
    material->name = name;
    m_materials[name] = material;
    return material;
}

std::shared_ptr<Material> MaterialManager::getMaterial(const std::string& name) {
    auto it = m_materials.find(name);
    return (it != m_materials.end()) ? it->second : nullptr;
}

void MaterialManager::removeMaterial(const std::string& name) {
    m_materials.erase(name);
}

const std::unordered_map<std::string, std::shared_ptr<Material>>& MaterialManager::getAllMaterials() const {
    return m_materials;
}
