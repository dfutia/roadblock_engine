#pragma once
#ifndef SCRIPT_EDITOR_H
#define SCRIPT_EDITOR_H

#include "Editor/editorpanel.h"
#include "instance.h"

#include <imgui.h>
#include <imguitextedit/TextEditor.h>

#include <string>
#include <unordered_map>

class ScriptEditor : public EditorPanel {
public:
    ScriptEditor() {
        m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
        m_editor.SetPalette(TextEditor::GetDarkPalette());
    }

    void render() override {
        ImGui::Begin("Script Editor");

        if (m_currentScript) {
            if (ImGui::Button("Save")) {
                saveScript();
            }
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                closeScript();
            }

            m_editor.Render("TextEditor");
        }
        else {
            ImGui::Text("No script open");
        }

        ImGui::End();
    }

    void openScript(Instance* scriptInstance) {
        if (scriptInstance && scriptInstance->name == "Script") {
            m_currentScript = scriptInstance;

            auto it = m_scriptContents.find(scriptInstance);
            if (it != m_scriptContents.end()) {
                m_editor.SetText(it->second);
            }
            else {
                // If it's a new script, initialize with empty content
                m_editor.SetText("");
                m_scriptContents[scriptInstance] = "";
            }
        }
    }
private:
    TextEditor m_editor;
    Instance* m_currentScript = nullptr;
    std::unordered_map<Instance*, std::string> m_scriptContents;

    void saveScript() {
        if (m_currentScript) {
            m_scriptContents[m_currentScript] = m_editor.GetText();
            // Here you would typically save the script to disk or update it in your game engine
        }
    }

    void closeScript() {
        m_currentScript = nullptr;
        m_editor.SetText("");
    }
};

#endif 