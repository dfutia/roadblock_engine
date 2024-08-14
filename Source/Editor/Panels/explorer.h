#pragma once
#ifndef EXPLORER_H
#define EXPLORER_H

#include "Editor/editorpanel.h"

#include <imgui.h>

class Explorer : public EditorPanel {
    void render() override {
        ImGui::Begin("Explorer");
        ImGui::End();
    }
};

#endif 