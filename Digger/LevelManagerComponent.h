#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include <glm.hpp>

namespace dae
{
	class GameObject;
	class TileTrackerComponent;
	class MoneyBagComponent;

	class LevelManagerComponent : public Component
	{
	public:
		void RegisterMoneyBag(std::shared_ptr<GameObject> moneyBag);
		std::shared_ptr<GameObject> GetMoneyBagAt(int col, int row) const;

		void Update(float deltaTime) override;

	private:
		std::vector<std::shared_ptr<GameObject>> m_MoneyBags;
	};
}

