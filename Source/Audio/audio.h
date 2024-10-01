#pragma once
#ifndef AUDIO_H
#define AUDIO_H

#include <SDL_mixer.h>

#include <iostream>
#include <string>
#include <vector>

struct AudioClip {
	std::string name;
	Mix_Music* music;
	bool isPlaying;
};

class Audio {
public:
	Audio() {
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	}

	~Audio() {
		for (auto& clip : m_audioClips) {
			Mix_FreeMusic(clip.music);
		}
		Mix_CloseAudio();
	}

	void loadAudio(std::string& filepath) {
		Mix_Music* newMusic = Mix_LoadMUS(filepath.c_str());
		if (newMusic) {
			std::string fileName = filepath.substr(filepath.find_last_of("/\\") + 1);
			m_audioClips.push_back({ fileName, newMusic, false });
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

	std::vector<AudioClip>& getLoadedClips() {
		return m_audioClips;
	}

	void setVolume(float volume) {
		m_volume = volume;
		Mix_VolumeMusic(static_cast<int>(m_volume * MIX_MAX_VOLUME));
	}

	float getVolume() const {
		return m_volume;
	}
private:
	std::vector<AudioClip> m_audioClips;
	Mix_Music* m_currentAudio = nullptr;
	float m_volume = 1.0f;
};

#endif 
