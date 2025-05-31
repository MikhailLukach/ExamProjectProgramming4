#pragma once
#include "Component.h"
#include <memory>
#include <glm.hpp>

namespace dae
{
	class GameObject;
	class TileManagerComponent;
	class TileTrackerComponent;
	class LevelLoader;

	class NobbinControllerComponent final : public Component
	{
	public:
		NobbinControllerComponent(GameObject* player,
			TileManagerComponent* tileManager,
			LevelLoader* levelLoader,
			float decisionInterval = 0.4f, float speed = 2.5f);

		void Update(float deltaTime) override;
	private:
		GameObject* m_pPlayer;
		TileManagerComponent* m_pTileManager;
		TileTrackerComponent* m_pTracker = nullptr;
		LevelLoader* m_pLevelLoader = nullptr;

		glm::ivec2 m_PreviousTile{};

		float m_DecisionTimer = 0.f;
		float m_DecisionInterval;
		float m_Speed;

		void TryMoveInDirection(const glm::ivec2& direction);
		bool IsDugTile(const glm::ivec2& tile) const;
	};
}

