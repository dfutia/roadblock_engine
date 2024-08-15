#pragma once
#ifndef SCRIPT_EDITOR_H
#define SCRIPT_EDITOR_H

#include "Editor/scripteditorwindow.h"

#include <iostream>
#include <vector>
#include <memory>

class ScriptEditor {
public:
	void openScript(Script* script) {
        auto it = std::find_if(m_scriptWindows.begin(), m_scriptWindows.end(),
            [script](const auto& window) { return window->getScript() == script; });
        if (it == m_scriptWindows.end()) {
            m_scriptWindows.push_back(std::make_unique<ScriptEditorWindow>(script));
            std::cout << "Opened new script: " << script->name << std::endl;
        }
        else {
            std::cout << "Script already open: " << script->name << std::endl;
        }
        std::cout << "Total open scripts: " << m_scriptWindows.size() << std::endl;
	}

    void render() {
        for (auto it = m_scriptWindows.begin(); it != m_scriptWindows.end();) {
            if ((*it)->isOpen()) {
                (*it)->render();
                ++it;
            }
            else {
                it = m_scriptWindows.erase(it);
            }
        }
    }
private:
	std::vector<std::unique_ptr<ScriptEditorWindow>> m_scriptWindows;
};

#endif 