#pragma once
#ifndef MESH_IMPORTER_H
#define MESH_IMPORTER_H

#include "Editor/editorpanel.h"

class MeshImporter : public EditorPanel {
public:
    MeshImporter(bool open = false, std::string name = "Mesh Importer") : EditorPanel(open, name) {}

    void render() override {
        ImGui::Begin(m_name.c_str(), &m_open);

        ImGui::End();
    }
};

#endif 
