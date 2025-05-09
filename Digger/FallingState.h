#pragma once
#include "MoneyBagState.h"

namespace dae
{
	class FallingState : public MoneyBagState
	{
	public:
		void OnEnter(MoneyBagComponent& bag) override;
		std::unique_ptr<MoneyBagState> Update(MoneyBagComponent& bag, float deltaTime) override;
		void OnExit(MoneyBagComponent& bag) override;
	private:
		float m_FallTimer = 0.0f;
	};
}
