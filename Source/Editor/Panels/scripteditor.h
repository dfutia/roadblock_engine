#pragma once
#ifndef SCRIPT_EDITOR_H
#define SCRIPT_EDITOR_H

#include "Editor/editorpanel.h"
#include "Scene/Nodes/script.h"

#include <imgui.h>
#include <imguitextedit/TextEditor.h>

#include <sstream>
#include <string>

class ScriptEditor : public EditorPanel {
public:
    ScriptEditor(Script& script, ImFont* largeFont) :
        EditorPanel(true, "Script Editor"),
        m_script(script),
        m_largeFont(largeFont)
    {
        m_editor.SetLanguageDefinition(::TextEditor::LanguageDefinition::Lua());
        m_script.loadContent();
        m_editor.SetText(m_script.content);

        std::stringstream ss;
        ss << "Script Editor - " << m_script.getName() << " ##" << reinterpret_cast<uintptr_t>(&m_script);
        m_panelName = ss.str();
    }

    void render() override {
        ImGui::PushFont(ImGui::GetIO().FontDefault);

        ImGui::Begin(m_panelName.c_str());

        //float oldScale = ImGui::GetFont()->Scale;
        //ImGui::GetFont()->Scale = 1.5f; 
        //ImGui::PushFont(ImGui::GetFont());
        ImGui::PushFont(m_largeFont);
        m_editor.Render("TextEditor");
        ImGui::PopFont();

        if (ImGui::Button("Save")) {
            m_script.content = m_editor.GetText();
            m_script.saveContent();
        }

        //ImGui::GetFont()->Scale = oldScale;

        ImGui::End();

        ImGui::PopFont();
    }

    const Script& getScript() const { return m_script; }
private:
    std::string m_panelName;
    Script& m_script;
    TextEditor m_editor;
    ImFont* m_largeFont;
};

#endif 