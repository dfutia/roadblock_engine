#pragma once
#ifndef EXPLORER_H
#define EXPLORER_H

#include "Editor/editorpanel.h"
#include "Editor/editorcontext.h"
#include "Scene/SceneGraph/instance.h"
#include "Scene/SceneGraph/model.h"
#include "Scene/SceneGraph/part.h"
#include "Scene/SceneGraph/script.h"

#include <imgui.h>

#include <memory>
#include <functional>
#include <iostream>

class Explorer : public EditorPanel {
public:
    Explorer(EditorContext& editorContext) : m_editorContext(editorContext), root(std::make_unique<Instance>()) {
        root->name = "Scene";
    }

    void render() override {
        ImGui::Begin("Explorer");

        renderTree(root.get());

        ImGui::End();
    }
private:
    EditorContext& m_editorContext;
    std::unique_ptr<Instance> root;

    //if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
    //    std::cout << "click" << std::endl;
    //}

    //if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0)) {
    //    std::cout << "double click" << std::endl;
    //}

    void renderTree(Instance* instance) {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (instance->children.empty()) {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }
        bool opened = ImGui::TreeNodeEx((void*)(intptr_t)instance, flags, "%s", instance->name.c_str());

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            m_editorContext.selected = instance;
        }

        if (ImGui::IsItemClicked() && ImGui::IsMouseDoubleClicked(0)) {
            if (auto* script = dynamic_cast<Script*>(instance)) {
                EngineEvents::OpenScriptEvent.Fire(*script);
            }
        }

        // Drag source
        if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
            ImGui::SetDragDropPayload("INSTANCE_DRAG", &instance, sizeof(Instance*));
            ImGui::Text("Moving %s", instance->name.c_str());
            ImGui::EndDragDropSource();
        }

        // Drop target
        if (ImGui::BeginDragDropTarget()) {
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("INSTANCE_DRAG")) {
                IM_ASSERT(payload->DataSize == sizeof(Instance*));
                Instance* movedInstance = *(Instance**)payload->Data;
                if (movedInstance != instance && !isAncestor(movedInstance, instance)) {
                    movedInstance->setParent(instance);
                }
            }
            ImGui::EndDragDropTarget();
        }

        std::string popupId = "ContextMenu_" + std::to_string(reinterpret_cast<intptr_t>(instance));
        if (ImGui::BeginPopupContextItem(popupId.c_str())) {
            if (ImGui::MenuItem("Add Script")) {
                m_editorContext.action = EditorAction::ADD_SCRIPT;
                m_editorContext.targetInstance = instance;
            }
            if (ImGui::MenuItem("Add Part")) {
                m_editorContext.action = EditorAction::ADD_PART;
                m_editorContext.targetInstance = instance;
            }
            if (ImGui::MenuItem("Add Model")) {
                m_editorContext.action = EditorAction::ADD_MODEL;
                m_editorContext.targetInstance = instance;
            }
            ImGui::EndPopup();
        }

        if (opened) {
            for (auto child : instance->children) {
                renderTree(child);
            }
            ImGui::TreePop();
        }
    }

    bool isAncestor(Instance* potentialAncestor, Instance* instance) {
        Instance* parent = instance->parent;
        while (parent != nullptr) {
            if (parent == potentialAncestor) {
                return true;
            }
            parent = parent->parent;
        }
        return false;
    }
};





#endif 