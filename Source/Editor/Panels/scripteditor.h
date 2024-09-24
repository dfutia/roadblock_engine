#pragma once
#ifndef SCRIPT_EDITOR_H
#define SCRIPT_EDITOR_H

#include "Editor/editorpanel.h"
#include "Scene/SceneGraph/script.h"

#include <imgui.h>
#include <imguitextedit/TextEditor.h>

#include <sstream>
#include <string>

class ScriptEditor : public EditorPanel {
public:
    ScriptEditor(Script& script) : 
        EditorPanel(true, "Script Editor"),
        m_script(script) 
    {
        m_editor.SetLanguageDefinition(::TextEditor::LanguageDefinition::Lua());
        m_script.loadContent();
        m_editor.SetText(m_script.content);

        std::stringstream ss;
        ss << "Script Editor - " << m_script.name << " ##" << reinterpret_cast<uintptr_t>(&m_script);
        m_panelName = ss.str();
    }

    void render() override {
        ImGui::Begin(m_panelName.c_str());

        float oldScale = ImGui::GetFont()->Scale;
        ImGui::GetFont()->Scale = 1.5f; 
        ImGui::PushFont(ImGui::GetFont());

        m_editor.Render("TextEditor");

        if (ImGui::Button("Save")) {
            m_script.content = m_editor.GetText();
            m_script.saveContent();
        }

        ImGui::PopFont();
        ImGui::GetFont()->Scale = oldScale;

        ImGui::End();
    }

    Script& getScript() { return m_script; }
private:
   std::string m_panelName;
    Script& m_script;
    TextEditor m_editor;
};

#endif 