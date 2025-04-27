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
	};
}