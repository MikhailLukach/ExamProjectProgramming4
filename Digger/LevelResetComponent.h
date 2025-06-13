#pragma once
#include "Component.h"
#include "Observer.h"
#include <functional>

namespace dae
{
	class LevelResetComponent final : public Component, public Observer
	{
	public:
		LevelResetComponent(std::function<void(int)> reloadFunc, bool isCoop = false);

		void Update(float deltaTime) override;

		void Notify(EventId event, GameObject* gameObject) override;

		void QueueResetToLevel(int levelIndex);

	private:
		std::function<void(int)> m_ReloadFunc;

		bool m_Waiting = false;
		float m_Timer = 0.0f;
		const float kDelay = 5.0f;

		int m_DeathsSeen = 0;
		int m_DeathThreshold;

		int m_TargetLevel = 1;
	};
}

