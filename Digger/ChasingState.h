#pragma once
#include "NobbinState.h"

namespace dae
{
	class ChasingState : public NobbinState
	{
	public:
		void OnEnter(NobbinControllerComponent& controller) override;
		void OnExit(NobbinControllerComponent& controller) override;

		void Update(NobbinControllerComponent& controller, float deltaTime) override;
	private:
		float m_AngerTimer = 0.f;
		const float m_AngerThreshold = 5.f;
	};
}

