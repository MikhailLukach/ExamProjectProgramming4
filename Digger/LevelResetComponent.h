#pragma once
#include "Component.h"
#include "Observer.h"
#include <functional>

namespace dae
{
	class LevelResetComponent final : public Component, public Observer
	{
	public:
		LevelResetComponent(std::function<void()> reloadFunc);

		void Update(float deltaTime) override;

		void Notify(EventId event, GameObject* gameObject) override;

	private:
		std::function<void()> m_ReloadFunc;

		bool m_Waiting = false;
		float m_Timer = 0.0f;
		const float kDelay = 5.0f;
	};
}

