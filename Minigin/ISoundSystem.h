#pragma once
#include <string>

namespace dae
{
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void PlayMusic(const std::string& musicFile, int loops = -1) = 0;
		virtual void PlaySound(const std::string& soundFile) = 0;
		virtual void StopMusic() = 0;
	};
}
