#pragma once
#ifndef EXPLORER_H
#define EXPLORER_H

#include "Editor/editorpanel.h"
#include "instance.h"

#include <imgui.h>

#include <memory>
#include <functional>
#include <iostream>

class Explorer : public EditorPanel {
public:
    Explorer() : root(std::make_unique<Instance>()) {
        root->name = "Scene";
        auto script = new Script("Script");
        script->setParent(root.get());
        auto script2 = new Script("Script");
        script2->setParent(root.get());
        auto script3 = new Script("Script");
        script3->setParent(root.get());
    }

    void render() override {
        ImGui::Begin("Explorer");
        renderTree(root.get());
        ImGui::End();
    }
private:
    std::unique_ptr<Instance> root;

    void renderTree(Instance* instance) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (instance->children.empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        bool opened = ImGui::TreeNodeEx(instance->name.c_str(), flags);

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            if (instance->name == "Script") {
                Script* script = dynamic_cast<Script*>(instance);
                EngineEvents::OpenScriptEvent.Fire(*script);
            }
        }

        if (opened) {
            for (auto child : instance->children) {
                renderTree(child);
            }
            ImGui::TreePop();
        }
    }
};

#endif 