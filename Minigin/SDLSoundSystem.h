#pragma once
#include "ISoundSystem.h"
#include <string>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <SDL_mixer.h>
#include <unordered_map>

namespace dae
{
	class SDLSoundSystem : public ISoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem() override;

		void PlaySound(const std::string& soundFile) override;
		void PlayMusic(const std::string& musicFile, int loops = -1) override;
		void StopMusic() override;

		void SetMuted(bool mute) override;

	private:
		void ProcessQueue();
		Mix_Chunk* GetOrLoadChunk(const std::string& file);

		std::thread m_Thread;
		std::queue<std::string> m_SoundQueue;
		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;
		std::atomic<bool> m_Running{ true };

		std::unordered_map<std::string, Mix_Chunk*> m_SoundCache;
	};
}

