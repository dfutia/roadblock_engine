#pragma once
#ifndef PROPERTIES_H
#define PROPERTIES_H

#include "Editor/editorpanel.h"

#include <imgui.h>

class Properties : public EditorPanel {
    void render() override {
        ImGui::Begin("Properties");
        ImGui::End();
    }
};

#endif 