#pragma once
#include <string>

namespace dae
{
	class ISoundSystem
	{
	public:
		virtual ~ISoundSystem() = default;
		virtual void PlaySound(const std::string& soundFile) = 0;
	};
}
