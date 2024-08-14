#pragma once
#ifndef CONSOLE_H
#define CONSOLE_H

#include "Editor/editorpanel.h"

#include <imgui.h>

class Console : public EditorPanel {
    void render() override {
        ImGui::Begin("Console");
        ImGui::End();
    }
};

#endif 