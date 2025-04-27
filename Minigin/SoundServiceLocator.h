#pragma once
#include "ISoundSystem.h"
#include "NullSoundSystem.h"

namespace dae
{
	class SoundServiceLocator final
	{
	public:
		static ISoundSystem& Get()
		{
			if (!m_pService)
				m_pService = new NullSoundSystem{};
			return *m_pService;
		}

		static void Provide(ISoundSystem* service)
		{
			m_pService = service ? service : new NullSoundSystem{};
		}

		static void Cleanup()
		{
			delete m_pService;
			m_pService = nullptr;
		}

	private:
		static inline ISoundSystem* m_pService = nullptr;
	};
}