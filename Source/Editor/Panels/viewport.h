#pragma once
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Editor/editorpanel.h"

#include <imgui.h>

class Viewport : public EditorPanel {
    void render() override {
        ImGui::Begin("Viewport");
        ImGui::End();
    }
};

#endif 