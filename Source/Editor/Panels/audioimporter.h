#pragma once
#ifndef AUDIO_IMPORTER_H
#define AUDIO_IMPORTER_H

#include "Editor/editorpanel.h"
#include "filesystem.h"

#include <SDL_mixer.h>

#include <iostream>
#include <string>
#include <vector>

class AudioImporter : public EditorPanel {
public:
    AudioImporter(bool open = false, std::string name = "Audio Importer") : EditorPanel(open, name) {}

    void render() override {
        ImGui::Begin(m_name.c_str(), &m_open);

        if (ImGui::Button("Import Audio")) {
            std::wstring filepath = gFilesystem.openFileDialog();
            std::string path(filepath.begin(), filepath.end());
            loadAudio(path);
        }

        if (!m_audioClips.empty()) {
            ImGui::Text("Loaded Audio Clips:");
            for (int i = 0; i < m_audioClips.size(); ++i) {
                ImGui::Text("%s", m_audioClips[i].name.c_str());
                ImGui::SameLine();
                if (ImGui::Button(("Play##" + std::to_string(i)).c_str())) {
                    playAudio(i);
                }
                ImGui::SameLine();
                if (ImGui::Button("Pause")) {
                    pauseAudio();
                }
            }
        }

        ImGui::End();
    }

    void loadAudio(const std::string& filePath) {
        std::cout << "loading audio" << std::endl;
        Mix_Music* newMusic = Mix_LoadMUS(filePath.c_str());
        if (newMusic) {
            std::string fileName = filePath.substr(filePath.find_last_of("/\\") + 1);
            m_audioClips.push_back({ fileName, newMusic, false });
            std::cout << "clip added" << std::endl;
        }
    }

    void playAudio(int index) {
        if (m_currentAudio) {
            Mix_HaltMusic();
        }
        m_currentAudio = m_audioClips[index].music;
        Mix_PlayMusic(m_currentAudio, -1);
        m_audioClips[index].isPlaying = true;
    }

    void pauseAudio() {
        if (m_currentAudio && Mix_PlayingMusic()) {
            Mix_PauseMusic();
            for (auto& clip : m_audioClips) {
                clip.isPlaying = false;
            }
        }
    }

private:
    struct AudioClip {
        std::string name;
        Mix_Music* music;
        bool isPlaying;
    };

    std::vector<AudioClip> m_audioClips;
    Mix_Music* m_currentAudio = nullptr;
};

#endif 
