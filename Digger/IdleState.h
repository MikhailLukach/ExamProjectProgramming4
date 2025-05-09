#pragma once
#include "MoneyBagState.h"

namespace dae
{
	class IdleState : public MoneyBagState
	{
	public:
		void OnEnter(MoneyBagComponent& bag) override;
		std::unique_ptr<MoneyBagState> Update(MoneyBagComponent& bag, float deltaTime) override;
		void OnExit(MoneyBagComponent& bag) override;
	};
}

