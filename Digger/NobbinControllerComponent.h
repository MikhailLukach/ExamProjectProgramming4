#pragma once
#include "Component.h"
#include "GameObject.h"
#include <memory>
#include <glm.hpp>

namespace dae
{
	enum class NobbinAnimationState
	{
		WalkRight,
		WalkDown,
		WalkLeft,
		WalkUp
	};

	struct AStarNode
	{
		glm::ivec2 tile;
		float      gCost = 0.0f;
		float      hCost = 0.0f;
		int        parentIndex = -1;
		float      fCost() const { return gCost + hCost; }
	};

	class TileManagerComponent;
	class LevelManagerComponent;
	class TileTrackerComponent;
	class SpriteAnimatorComponent;
	class LevelLoader;
	class NobbinState;

	class NobbinControllerComponent : public Component
	{
	public:
		NobbinControllerComponent(GameObject* player,
			TileManagerComponent* tileManager,
			LevelManagerComponent* levelManager,
			LevelLoader* levelLoader,
			float decisionInterval = 0.4f, float speed = 2.5f);

		void Update(float deltaTime) override;

		void ChangeState(std::unique_ptr<NobbinState> newState);

		GameObject* GetPlayer() const { return m_pPlayer; }
		TileManagerComponent* GetTileManager() const { return m_pTileManager; }
		TileTrackerComponent* GetTracker()
		{
			if (!m_pTracker)
				m_pTracker = GetOwner()->GetComponent<TileTrackerComponent>().get();
			return m_pTracker;
		}

		glm::ivec2 GetPreviousTile() const { return m_PreviousTile; }

		float GetSpeed() const { return m_Speed; }

		LevelLoader* GetLevelLoader() const { return m_pLevelLoader; }

		LevelManagerComponent* GetLevelManager() const { return m_pLevelManager; }

		void TryMoveInDirection(const glm::ivec2& direction);
		bool IsDugTile(const glm::ivec2& tile) const;

		int GetTileCost(const glm::ivec2& tile) const;

		std::vector<glm::ivec2> FindPath(const glm::ivec2& start, const glm::ivec2& goal);
	private:
		GameObject* m_pPlayer;
		TileManagerComponent* m_pTileManager;
		LevelManagerComponent* m_pLevelManager = nullptr;

		TileTrackerComponent* m_pTracker = nullptr;
		SpriteAnimatorComponent* m_pAnimator = nullptr;
		LevelLoader* m_pLevelLoader = nullptr;

		std::unique_ptr<NobbinState> m_pCurrentState;

		glm::ivec2 m_PreviousTile{};
		glm::ivec2 m_LastMoveDir{};
		NobbinAnimationState m_AnimState{ NobbinAnimationState::WalkLeft };
		NobbinAnimationState m_LastAnimState{ NobbinAnimationState::WalkLeft };

		float m_DecisionTimer = 0.f;
		float m_DecisionInterval;
		float m_Speed;

		bool m_Initialized = false;

		bool m_HasHitPlayer = false;

		void UpdateAnimationState();
	};
}

