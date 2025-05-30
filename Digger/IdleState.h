#pragma once
#include "MoneyBagState.h"

namespace dae
{
	class TileManagerComponent;
	class IdleState : public MoneyBagState
	{
	public:
		void OnEnter(MoneyBagComponent& bag) override;

		std::unique_ptr<MoneyBagState> Update(MoneyBagComponent& bag, float deltaTime) override;

		void OnExit(MoneyBagComponent& bag) override;

		void TryPushHorizontally(MoneyBagComponent& bag, int direction, TileManagerComponent* tileManager) override;
	private:
		float m_FallDelayTimer{ -1.f };
	};
}

