#include "SDLSoundSystem.h"
#include <iostream>
#include <SDL_timer.h>


dae::SDLSoundSystem::SDLSoundSystem()
{
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << '\n';
    }
    else
    {
        std::cout << "SDL_mixer initialized successfully!\n";
    }

	m_Thread = std::thread(&SDLSoundSystem::ProcessQueue, this);
}

dae::SDLSoundSystem::~SDLSoundSystem()
{
	for (auto& pair : m_SoundCache)
	{
		Mix_FreeChunk(pair.second);
	}
	m_SoundCache.clear();

    m_Running = false;
    m_Condition.notify_one();
    if (m_Thread.joinable())
        m_Thread.join();

	std::queue<std::string> empty;
	std::swap(m_SoundQueue, empty);
    Mix_CloseAudio();
}

void dae::SDLSoundSystem::PlaySound(const std::string& soundFile)
{
	std::cout << "[SDLSoundSystem] Queuing: " << soundFile << "\n";
    {
        std::lock_guard lock(m_QueueMutex);
        m_SoundQueue.push(soundFile);
    }
    m_Condition.notify_one();
}

void dae::SDLSoundSystem::PlayMusic(const std::string& musicFile, int loops)
{
	Mix_Music* music = Mix_LoadMUS(musicFile.c_str());
	if (!music)
	{
		std::cerr << "Failed to load music: " << Mix_GetError() << '\n';
		return;
	}

	if (Mix_PlayMusic(music, loops) == -1)
	{
		std::cerr << "Failed to play music: " << Mix_GetError() << '\n';
		Mix_FreeMusic(music);
		return;
	}
}


void dae::SDLSoundSystem::ProcessQueue()
{
	while (m_Running)
	{
		std::unique_lock lock(m_QueueMutex);
		m_Condition.wait(lock, [this] { return !m_SoundQueue.empty() || !m_Running; });

		while (!m_SoundQueue.empty())
		{
			std::string file = m_SoundQueue.front();
			m_SoundQueue.pop();
			std::cout << "[SDLSoundSystem] Playing: " << file << "\n";
			lock.unlock();

			/*Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
			if (!chunk)
			{
				std::cerr << "Failed to load sound: " << file << ", error: " << Mix_GetError() << "\n";
			}
			else
			{
				Mix_PlayChannel(-1, chunk, 0);
				while(Mix_Playing(-1))
				{
					SDL_Delay(10);
				}
				Mix_FreeChunk(chunk);
			}*/

			Mix_Chunk* chunk = GetOrLoadChunk(file);
			if (chunk)
			{
				Mix_PlayChannel(-1, chunk, 0);
			}

			lock.lock();
		}
	}
}

Mix_Chunk* dae::SDLSoundSystem::GetOrLoadChunk(const std::string& file)
{
	auto it = m_SoundCache.find(file);
	if (it != m_SoundCache.end())
		return it->second;

	Mix_Chunk* chunk = Mix_LoadWAV(file.c_str());
	if (!chunk)
	{
		std::cerr << "Failed to load sound: " << file << ", error: " << Mix_GetError() << "\n";
		return nullptr;
	}

	m_SoundCache[file] = chunk;
	return chunk;
}
