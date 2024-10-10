#pragma once
#ifndef EXPLORER_H
#define EXPLORER_H

#include "Editor/editorpanel.h"
#include "Editor/editor.h"
#include "Editor/editorcontext.h"
#include "Scene/Nodes/instance.h"
#include "Scene/Nodes/model.h"
#include "Scene/Nodes/part.h"
#include "Scene/Nodes/script.h"
#include "Reflection/typeregistry.h"

#include <imgui.h>

#include <memory>
#include <functional>
#include <iostream>

class Explorer : public EditorPanel {
public:
    Event<Script&> openScriptEvent;

    Explorer(Editor& editor, EditorContext& editorContext) : 
        EditorPanel(true, "Explorer"),
        m_editor(editor), 
        m_editorContext(editorContext), 
        root(std::make_unique<Instance>()) 
    {
        root->name = "Scene";
        m_editorContext.test = root.get();
    }

    void render() override {
        ImGui::Begin("Explorer", &m_open);

        renderTree(root.get());

        ImGui::End();
    }
private:
    Editor& m_editor;
    EditorContext& m_editorContext;
    std::unique_ptr<Instance> root;

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
                openScriptEvent.Fire(*script);
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
            for (const std::string& typeName : gTypeRegistry.getRegisteredTypes()) {
                if (ImGui::MenuItem((typeName).c_str())) {
                    m_editorContext.action = EditorAction::ADD_INSTANCE;
                    m_editorContext.targetInstance = instance;
                    m_editorContext.instanceTypeToAdd = typeName;
                }
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