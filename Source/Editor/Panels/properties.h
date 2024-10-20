#pragma once
#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "Editor/editorpanel.h"
#include "Editor/editorcontext.h"
#include "Scene/Nodes/instance.h"
#include "Scene/Nodes/script.h"
#include "Scene/Nodes/model.h"
#include "Scene/Nodes/part.h"
#include "Scene/Nodes/meshpart.h"
#include "Reflection/property.h"

#include <imgui.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

#include <string>
#include <map>
#include <vector>

class Properties : public EditorPanel {
public:
    Properties(EditorContext& editorContext) : 
        EditorPanel(true, "Properties"),
        m_editorContext(editorContext) {}

    void render() override {
        ImGui::Begin("Properties", &m_open);

        Instance* selected = m_editorContext.selected;
        if (selected) {
            ImGui::Text("Type: %s", selected->getTypeName().c_str());

            displayProperties(selected);
        }

        ImGui::End();
    }
private:
    EditorContext& m_editorContext;

    void displayProperties(Instance* instance) {
        const auto& properties = instance->getProperties();

        std::map<std::string, std::vector<const Property*>> sections;

        // Group properties by section
        for (const auto& prop : properties) {
            sections[prop->getSection()].push_back(prop.get());
        }

        // Display properties by section
        for (const auto& [sectionName, props] : sections) {
            if (ImGui::CollapsingHeader(sectionName.c_str())) {
                for (const auto& prop : props) {
                    displayProperty(instance, prop);
                }
            }
        }
    }

    void displayProperty(Instance* instance, const Property* prop) {
        const std::string& propName = prop->getName();
        const std::string& typeName = prop->getTypeName();

        //std::cout << "Property Name: '" << propName << "', Type: '" << typeName << "'" << std::endl;

        if (typeName == "std::string") {
            std::string* value = static_cast<std::string*>(prop->getValue(instance));
            std::string uniqueLabel = propName + "##" + std::to_string(reinterpret_cast<uintptr_t>(prop));
            char buffer[256];
            strncpy_s(buffer, sizeof(buffer), value->c_str(), _TRUNCATE); // Use strncpy_s instead of strncpy
            if (ImGui::InputText(uniqueLabel.c_str(), buffer, sizeof(buffer))) {
                *value = buffer;
                prop->setValue(instance, value);
            }
        }
        else if (typeName == "float") {
            float* value = static_cast<float*>(prop->getValue(instance));
            std::string uniqueLabel = propName + "##" + std::to_string(reinterpret_cast<uintptr_t>(prop));
            if (ImGui::DragFloat(uniqueLabel.c_str(), value, 0.1f)) {
                prop->setValue(instance, value);
            }
        }
        else if (typeName == "glm::vec3") {
            glm::vec3* value = static_cast<glm::vec3*>(prop->getValue(instance));
            std::string uniqueLabel = propName + "##" + std::to_string(reinterpret_cast<uintptr_t>(prop));
            if (ImGui::DragFloat3(uniqueLabel.c_str(), glm::value_ptr(*value), 0.1f)) {
                prop->setValue(instance, value);
            }
        }
        else if (typeName == "glm::vec4") {
            glm::vec4* value = static_cast<glm::vec4*>(prop->getValue(instance));
            std::string uniqueLabel = propName + "##" + std::to_string(reinterpret_cast<uintptr_t>(prop));
            if (ImGui::ColorEdit4(uniqueLabel.c_str(), glm::value_ptr(*value))) {
                prop->setValue(instance, value);
            }
        } 
        else if (typeName == "std::shared_ptr<MeshHandle>") {
            auto value = static_cast<std::shared_ptr<MeshHandle>*>(prop->getValue(instance));
            std::string uniqueLabel = propName + "##" + std::to_string(reinterpret_cast<uintptr_t>(prop));

            std::string displayName = value && *value ? (*value)->GetName() : "No Mesh";
            ImGui::InputText(uniqueLabel.c_str(), &displayName[0], displayName.size() + 1, ImGuiInputTextFlags_ReadOnly);

            if (ImGui::BeginDragDropTarget()) {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ASSET_DRAG")) {
                    if (payload->DataSize == sizeof(AssetHandle*)) {
                        AssetHandle* droppedAssetPtr = *(AssetHandle**)payload->Data;
                        if (droppedAssetPtr) {
                            // Convert back to shared_ptr
                            std::shared_ptr<AssetHandle> droppedAsset(droppedAssetPtr, [](AssetHandle*) {});
                            if (auto meshHandle = std::dynamic_pointer_cast<MeshHandle>(droppedAsset)) {
                                *value = meshHandle;
                                prop->setValue(instance, value);
                            }
                        }
                    }
                }
                ImGui::EndDragDropTarget();
            }
        }
        else {
            ImGui::Text("%s: Unsupported type %s", propName.c_str(), typeName.c_str());
        }
    }
};

#endif 