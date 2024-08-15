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
        auto script = new Script();
        script->setParent(root.get());
    }

    void render() override {
        ImGui::Begin("Explorer");
        renderTree(root.get());
        ImGui::End();
    }

    void setScriptClickCallback(std::function<void(Instance*)> callback) {
        scriptClickCallback = callback;
    }
private:
    std::unique_ptr<Instance> root;
    std::function<void(Instance*)> scriptClickCallback;

    void renderTree(Instance* instance) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (instance->children.empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        bool opened = ImGui::TreeNodeEx(instance->name.c_str(), flags);

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            if (instance->name == "Script" && scriptClickCallback) {
                scriptClickCallback(instance);
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