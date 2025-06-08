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
		void RegisterNobbin(std::shared_ptr<GameObject> nobbin);
		std::shared_ptr<GameObject> GetMoneyBagAt(int col, int row) const;
		std::shared_ptr<GameObject> GetNobbinAt(int col, int row) const;

		const std::vector<std::shared_ptr<GameObject>>& GetAllMoneyBags() const
		{
			return m_MoneyBags;
		}

		void Update(float deltaTime) override;

	private:
		std::vector<std::shared_ptr<GameObject>> m_MoneyBags;
		std::vector<std::shared_ptr<GameObject>> m_Nobbins;
	};
}

