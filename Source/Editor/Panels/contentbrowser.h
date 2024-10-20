#pragma once
#ifndef CONTENT_BROWSER_H
#define CONTENT_BROWSER_H

#include "Editor/editorpanel.h"
#include "filesystem.h"

#include <imgui.h>

#include <string>
#include <memory>
#include <unordered_map>

class ContentBrowser : public EditorPanel {
public:
    ContentBrowser() : EditorPanel(true, "Content Browser") {
        m_icons["unknown"] = g_assetManager.GetAsset<Texture>(FileSource{ "Asset/Icons/unknown.png" }, false);
        m_icons["mesh"] = g_assetManager.GetAsset<Texture>(FileSource{ "Asset/Icons/mesh.png" }, false);
        m_icons["material"] = g_assetManager.GetAsset<Texture>(FileSource{ "Asset/Icons/material.png" }, false);
        m_icons["texture"] = g_assetManager.GetAsset<Texture>(FileSource{ "Asset/Icons/texture.png" }, false);
    }

    void render() override {
        ImGui::Begin("Content Browser", &m_open);
        float padding = 16.0f;
        float thumbnailSize = 64.0f;
        float cellSize = thumbnailSize + padding;
        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) columnCount = 1;
        ImGui::Columns(columnCount, 0, false);

        if (!g_assetManager.getAllAssets().empty()) {
            for (const auto& [key, asset] : g_assetManager.getAllAssets()) {
                ImGui::PushID(key.c_str());

                // Determine icon based on asset type
                std::shared_ptr<Texture> icon = m_icons["unknown"]->Get();
                if (dynamic_cast<MeshHandle*>(asset.get())) icon = m_icons["mesh"]->Get();
                else if (dynamic_cast<MaterialHandle*>(asset.get())) icon = m_icons["material"]->Get();
                else if (dynamic_cast<TextureHandle*>(asset.get())) icon = m_icons["texture"]->Get();

                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
                ImGui::ImageButton(reinterpret_cast<ImTextureID>(static_cast<intptr_t>(icon->getId())),
                    ImVec2(thumbnailSize, thumbnailSize), ImVec2(0, 0), ImVec2(1, 1), 0);
                ImGui::PopStyleColor();

                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_None)) {
                    AssetHandle* assetPtr = asset.get();
                    ImGui::SetDragDropPayload("ASSET_DRAG", &assetPtr, sizeof(AssetHandle*));
                    ImGui::Text("Dragging %s", key.c_str());
                    ImGui::EndDragDropSource();
                }

                ImGui::TextWrapped(gFilesystem.getFilename(key).c_str());
                ImGui::NextColumn();
                ImGui::PopID();
            }
        }

        ImGui::Columns(1);
        ImGui::End();
    }
private:
    std::unordered_map<std::string, std::shared_ptr<TextureHandle>> m_icons;
};

#endif 