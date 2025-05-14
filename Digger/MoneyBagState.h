#pragma once
#include <memory>

namespace dae
{
	class MoneyBagComponent;
	class TileManagerComponent;
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

		virtual void TryPushHorizontally(MoneyBagComponent& bag, int direction, TileManagerComponent* tileManager)
		{
			(void)bag;
			(void)direction;
			(void)tileManager;
		}
	};
}

