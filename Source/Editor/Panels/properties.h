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

#include <imgui.h>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

class Properties : public EditorPanel {
public:
    Properties(EditorContext& editorContext) : 
        EditorPanel(true, "Properties"),
        m_editorContext(editorContext) {}

    void render() override {
        ImGui::Begin("Properties", &m_open);

        Instance* selected = m_editorContext.selected;

        Script* script = dynamic_cast<Script*>(selected);
        Model* model = dynamic_cast<Model*>(selected);
        Part* part = dynamic_cast<Part*>(selected);
        MeshPart* meshPart = dynamic_cast<MeshPart*>(selected);

        if (script) {
            displayScript(script);
        }

        if (model) {
            displayModel(model);
        }

        if (part) {
            displayPart(part);
        }

        if (meshPart) {
            displayMeshPart(meshPart);
        }
        ImGui::End();
    }
private:
    EditorContext& m_editorContext;

    void displayScript(Script* script) { 
        static std::string nameBuffer = script->name;
        if (ImGui::InputText("Name", &nameBuffer[0], nameBuffer.capacity())) {
            script->name = nameBuffer;
        }

        ImGui::Checkbox("Enabled", &script->enabled);
    }

    void displayModel(Model* model) {
        static std::string nameBuffer = model->name;
        if (ImGui::InputText("Name", &nameBuffer[0], nameBuffer.capacity())) {
            model->name = nameBuffer;
        }
    }

    void displayPart(Part* part) {
        static std::string nameBuffer = part->name;
        if (ImGui::InputText("Name", &nameBuffer[0], nameBuffer.capacity())) {
            part->name = nameBuffer;
        }

        glm::vec4 color = part->getColor();
        if (ImGui::ColorEdit4("Color", glm::value_ptr(color))) {
            part->setColor(color);
        }

        glm::mat4& transform = part->transform;

        glm::vec3 position, scale, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose(transform, scale, rotation, position, skew, perspective);

        glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(rotation));

        // Display and edit position
        if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f)) {
            // Update transform
            transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
        }

        // Display and edit rotation
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotationEuler), 0.1f)) {
            // Convert Euler angles back to quaternion
            rotation = glm::quat(glm::radians(rotationEuler));
            // Update transform
            transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
        }

        // Display and edit scale
        if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f, 0.01f, 100.0f)) {
            // Update transform
            transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
        }
    }

    void displayMeshPart(MeshPart* part) {
        static std::string nameBuffer = part->name;
        if (ImGui::InputText("Name", &nameBuffer[0], nameBuffer.capacity())) {
            part->name = nameBuffer;
        }

        glm::vec4 color = part->getColor();
        if (ImGui::ColorEdit4("Color", glm::value_ptr(color))) {
            part->setColor(color);
        }

        glm::mat4& transform = part->transform;

        glm::vec3 position, scale, skew;
        glm::quat rotation;
        glm::vec4 perspective;
        glm::decompose(transform, scale, rotation, position, skew, perspective);

        glm::vec3 rotationEuler = glm::degrees(glm::eulerAngles(rotation));

        // Display and edit position
        if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f)) {
            // Update transform
            transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
        }

        // Display and edit rotation
        if (ImGui::DragFloat3("Rotation", glm::value_ptr(rotationEuler), 0.1f)) {
            // Convert Euler angles back to quaternion
            rotation = glm::quat(glm::radians(rotationEuler));
            // Update transform
            transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
        }

        // Display and edit scale
        if (ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f, 0.01f, 100.0f)) {
            // Update transform
            transform = glm::translate(glm::mat4(1.0f), position) * glm::mat4_cast(rotation) * glm::scale(glm::mat4(1.0f), scale);
        }
    }
};

#endif 