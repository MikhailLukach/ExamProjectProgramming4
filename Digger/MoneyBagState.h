#pragma once
#include <memory>

namespace dae
{
	class MoneyBagComponent;
	class MoneyBagState
	{
	public:
		virtual ~MoneyBagState() = default;
		virtual void OnEnter(MoneyBagComponent& bag) 
		{
			(void)bag;
		}
		virtual void OnExit(MoneyBagComponent& bag) 
		{
			(void)bag;
		}
		virtual std::unique_ptr<MoneyBagState> Update(MoneyBagComponent& bag, float deltaTime) = 0;
	};
}

