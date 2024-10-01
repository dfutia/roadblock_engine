#pragma once
#ifndef AUDIO_IMPORTER_H
#define AUDIO_IMPORTER_H

#include "Editor/editorpanel.h"
#include "Audio/audio.h"
#include "filesystem.h"

#include <iostream>
#include <vector>

class AudioImporter : public EditorPanel {
public:
    AudioImporter(Audio& audio, bool open = false, std::string name = "Audio Importer") : EditorPanel(open, name), m_audio(audio) {}

    void render() override {
        ImGui::Begin(m_name.c_str(), &m_open);

        if (ImGui::Button("Import Audio")) {
            std::wstring filepath = gFilesystem.openFileDialog();
            std::string path(filepath.begin(), filepath.end());
            m_audio.loadAudio(path);
        }

        float volume = m_audio.getVolume();
        if (ImGui::SliderFloat("Volume", &volume, 0.0f, 1.0f)) {
            m_audio.setVolume(volume);
        }

        std::vector<AudioClip>& audioClips = m_audio.getLoadedClips();
        if (!audioClips.empty()) {
            ImGui::Text("Loaded Audio Clips:");
            for (int i = 0; i < audioClips.size(); ++i) {
                ImGui::Text("%s", audioClips[i].name.c_str());
                ImGui::SameLine();
                if (ImGui::Button(("Play##" + std::to_string(i)).c_str())) {
                    m_audio.playAudio(i);
                }
                ImGui::SameLine();
                if (ImGui::Button("Pause")) {
                    m_audio.pauseAudio();
                }
            }
        }

        ImGui::End();
    }
private:
    Audio& m_audio;
};

#endif 
