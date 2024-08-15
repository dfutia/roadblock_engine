#pragma once
#ifndef SCRIPT_EDITOR_WINDOW_H
#define SCRIPT_EDITOR_WINDOW_H

#include "instance.h"

#include <imguitextedit/TextEditor.h>

class ScriptEditorWindow {
public:
    ScriptEditorWindow(Script* script) : m_script(script) {
        m_editor.SetLanguageDefinition(TextEditor::LanguageDefinition::Lua());
        m_editor.SetText(script->content);
    }

    void render() {
        if (!m_isOpen) return;

        // Create a unique window name
        std::string windowName = "Script: " + m_script->name + "###" + std::to_string(reinterpret_cast<uintptr_t>(m_script));

        ImGui::SetNextWindowSize(ImVec2(400, 300), ImGuiCond_FirstUseEver);
        if (ImGui::Begin(windowName.c_str(), &m_isOpen)) {
            if (ImGui::Button("Save")) {
                saveScript();
            }
            ImGui::SameLine();
            if (ImGui::Button("Close")) {
                m_isOpen = false;
            }
            m_editor.Render("TextEditor");
        }
        ImGui::End();
    }

    bool isOpen() const { return m_isOpen; }
    void close() { m_isOpen = false; }
    Script* getScript() const { return m_script; }
    const std::string& getScriptName() const { return m_script->name; }
private:
    Script* m_script;
    TextEditor m_editor;
    bool m_isOpen = true;

    void saveScript() {
        m_script->content = m_editor.GetText();
    }
};

#endif 
