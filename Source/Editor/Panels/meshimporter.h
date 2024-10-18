#pragma once
#ifndef MESH_IMPORTER_H
#define MESH_IMPORTER_H

#include "Editor/editorpanel.h"
#include "Editor/editorcontext.h"
#include "Scene/scene.h"
#include "Scene/Nodes/meshpart.h"
#include "Asset/assetmanager.h"
#include "Asset/textureloader.h"
#include "Asset/materialloader.h"

#include <spdlog/spdlog.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>

class MeshImporter : public EditorPanel {
public:
    MeshImporter(Scene& scene, EditorContext& editorContext, bool open = false, std::string name = "Mesh Importer") : EditorPanel(open, name), m_scene(scene), m_editorContext(editorContext) {}

    void render() override {
        ImGui::Begin(m_name.c_str(), &m_open);

        if (ImGui::Button("Import Mesh")) {
            std::wstring filepath = gFilesystem.openFileDialog();
            if (!filepath.empty()) {
                std::string path(filepath.begin(), filepath.end());
                std::shared_ptr<MeshHandle> meshHandle = g_assetManager.GetAsset<Mesh>(FileSource{ path });

                std::string filename = path.substr(path.find_last_of("/\\") + 1);
                filename = filename.substr(0, filename.find_last_of('.'));

                auto meshPart = std::make_unique<MeshPart>(*meshHandle->Get());
                meshPart->setName(filename);
                meshPart->setParent(m_editorContext.test);

                m_scene.addInstance(std::move(meshPart));
            }
        }

        ImGui::End();
    }
private:
    EditorContext& m_editorContext;
    Scene& m_scene;
};

#endif 
