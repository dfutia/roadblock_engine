#pragma once
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "Editor/editorpanel.h"
#include "Graphic/renderer.h"

#include <imgui.h>

class Viewport : public EditorPanel {
public:
    Viewport(Renderer& renderer) : m_renderer(renderer) {}

    void render() override {
        ImGui::Begin("Viewport");
        // Get the size of the ImGui window content area
        ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        // Display the rendered texture
        ImGui::Image((void*)(intptr_t)m_renderer.getRenderedTextureID(), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
        ImGui::End();
    }
private:
    Renderer& m_renderer;
};

#endif 