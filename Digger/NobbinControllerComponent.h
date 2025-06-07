#pragma once
#include "Component.h"
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


	class GameObject;
	class TileManagerComponent;
	class TileTrackerComponent;
	class SpriteAnimatorComponent;
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
		SpriteAnimatorComponent* m_pAnimator = nullptr;
		LevelLoader* m_pLevelLoader = nullptr;

		glm::ivec2 m_PreviousTile{};
		NobbinAnimationState m_AnimState{ NobbinAnimationState::WalkLeft };
		NobbinAnimationState m_LastAnimState{ NobbinAnimationState::WalkLeft };

		float m_DecisionTimer = 0.f;
		float m_DecisionInterval;
		float m_Speed;

		void TryMoveInDirection(const glm::ivec2& direction);
		bool IsDugTile(const glm::ivec2& tile) const;
		void UpdateAnimationState();
	};
}

