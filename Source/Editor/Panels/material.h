#pragma once
#ifndef MATERIAL_PANEL_H
#define MATERIAL_PANEL_H

#include "Editor/editorpanel.h"
#include "Graphic/Resources/material.h"
#include "Graphic/Resources/texture.h"

#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <memory>

class MaterialPanel : public EditorPanel {
public:
    MaterialPanel() : EditorPanel(true, "Materials") {}

    void render() override {
        ImGui::Begin(m_name.c_str(), &m_open);

        if (ImGui::Button("Create New Material")) {
            createNewMaterial();
        }

        //const auto& materials = MaterialManager::getInstance().getAllMaterials();
        //if (!materials.empty()) {
        //    for (const auto& [name, material] : materials) {
        //        if (ImGui::TreeNode(name.c_str())) {
        //            renderMaterialProperties(material);
        //            ImGui::TreePop();
        //        }
        //    }
        //}

        ImGui::End();
    }
private:
    void createNewMaterial() {
        //static int counter = 0;
        //std::string name = "New Material " + std::to_string(counter++);
        //MaterialManager::getInstance().createMaterial(name);
    }

    void renderMaterialProperties(std::shared_ptr<Material> material) {
        ImGui::ColorEdit3("Ambient", glm::value_ptr(material->ambient));
        ImGui::ColorEdit3("Diffuse", glm::value_ptr(material->diffuse));
        ImGui::ColorEdit3("Specular", glm::value_ptr(material->specular));
        ImGui::SliderFloat("Shininess", &material->shininess, 0.0f, 256.0f);

        if (ImGui::TreeNode("Textures")) {
            for (size_t i = 0; i < material->textures.size(); ++i) {
                auto& texture = material->textures[i];
                //ImGui::Text("Texture %zu: %s", i, texture->name.c_str());
                ImGui::SameLine();
                ImGui::Image((void*)(intptr_t)texture->getId(), ImVec2(64, 64));
            }
            ImGui::TreePop();
        }
    }
};

#endif MATERIAL_PANEL_H
