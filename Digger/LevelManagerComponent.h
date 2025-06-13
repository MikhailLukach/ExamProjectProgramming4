#pragma once
#include "Component.h"
#include <algorithm>
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

		void RegisterPlayer(GameObject* player) {
			m_Players.push_back(player);
		}

		void UnregisterPlayer(GameObject* player)
		{
			auto it = std::find(m_Players.begin(), m_Players.end(), player);
			if (it != m_Players.end())
				m_Players.erase(it);
		}


		// Return all registered players
		const std::vector<GameObject*>& GetAllPlayers() const {
			return m_Players;
		}

	private:
		std::vector<std::shared_ptr<GameObject>> m_MoneyBags;
		std::vector<std::shared_ptr<GameObject>> m_Nobbins;
		std::vector<GameObject*> m_Players;

		int m_CurrentLevelIndex{ 1 };
		std::string m_LevelFilePattern{ "Level%d.lvl" };
		bool m_LoadedNextLevel{ false };
	};
}

