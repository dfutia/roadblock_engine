#pragma once
#ifndef CONTENT_BROWSER_H
#define CONTENT_BROWSER_H

#include "Editor/editorpanel.h"

#include <imgui.h>

class ContentBrowser : public EditorPanel {
    void render() override {
        ImGui::Begin("Content Browser");
        ImGui::End();
    }
};

#endif 