#pragma once
#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "Editor/editorpanel.h"
#include "Editor/editorcontext.h"
#include "Scene/SceneGraph/instance.h"
#include "Scene/SceneGraph/script.h"
#include "Scene/SceneGraph/model.h"
#include "Scene/SceneGraph/part.h"

#include <imgui.h>

class Properties : public EditorPanel {
public:
    Properties(EditorContext& editorContext) : m_editorContext(editorContext) {}

    void render() override {
        ImGui::Begin("Properties");

        Instance* selected = m_editorContext.selected;

        Script* script = dynamic_cast<Script*>(selected);
        Model* model = dynamic_cast<Model*>(selected);
        Part* part = dynamic_cast<Part*>(selected);

        if (script) {
            displayScript(script);
        }

        if (model) {
            displayModel();
        }

        if (part) {
            displayPart();
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

    void displayModel() { std::cout << "model properties" << std::endl; }
    void displayPart() { std::cout << "part properties" << std::endl; }
};

#endif 