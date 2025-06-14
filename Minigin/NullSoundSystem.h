#pragma once
#include "ISoundSystem.h"
#include <iostream>

namespace dae
{
	class NullSoundSystem : public ISoundSystem
	{
	public:
		void PlaySound(const std::string& soundFile) override
		{
			std::cout << "[NullSoundSystem] Would play sound: " << soundFile << "\n";
		}

		void PlayMusic(const std::string& musicFile, int loops = -1) override
		{
			std::cout << "[NullSoundSystem] Would play music: " << musicFile << " looping: " << loops << "\n";
		}

		void StopMusic() override
		{
			std::cout << "[NullSoundSystem] Would stop music " << "\n";
		}

		void  SetMuted(bool mute) override
		{
			std::cout << "[NullSoundSystem] Would mute music if: " << mute << "\n";
		}
	};
}