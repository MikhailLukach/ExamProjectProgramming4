#pragma once
#include "Component.h"
#include "FallingState.h"
#include "LevelManagerComponent.h"
#include "IdleState.h"
#include <memory>
#include <glm.hpp>
#include "BreakingState.h"
#include "CollectableState.h"

namespace dae
{
	class MoneyBagState;
	class TileManagerComponent;
	class MoneyBagComponent : public Component
	{
	public:
		MoneyBagComponent(LevelManagerComponent* levelManager);
		void Render() const override;

		void Update(float deltaTime) override;
		void SetState(std::unique_ptr<MoneyBagState> newState);

		void TryPushHorizontally(int direction, TileManagerComponent* tileManager);

		void StartMoveTo(const glm::vec3& targetPos, float duration = 0.25f);

		void AddFallDistance(int tiles) 
		{
			m_FallDistance += tiles;
		}

		int GetFallDistance() const
		{
			return m_FallDistance;
		}

		void ResetFallDistance()
		{
			m_FallDistance = 0;
		}

		void SetTileManager(TileManagerComponent* tileManager) 
		{ 
			m_pTileManager = tileManager; 
		}

		LevelManagerComponent* GetLevelManager() const 
		{
			return m_pLevelManager; 
		}

		TileManagerComponent* GetTileManager() const 
		{ 
			return m_pTileManager; 
		}

		glm::vec3 GetTargetPosition() const 
		{ 
			return m_TargetPosition; 
		}

		float GetMoveDuration() const 
		{
			return m_MoveDuration; 
		}

		float GetMoveElapsed() const
		{
			return m_MoveElapsed;
		}

		bool GetIsMoving()
		{
			return m_IsMoving;
		}

		bool IsFalling() const
		{
			return dynamic_cast<const FallingState*>(m_State.get()) != nullptr;
		}

		bool IsIdle() const
		{
			return dynamic_cast<const IdleState*>(m_State.get()) != nullptr;
		}

		bool IsBreaking() const
		{
			return dynamic_cast<const BreakingState*>(m_State.get()) != nullptr;
		}

		bool IsCollectable() const
		{
			return dynamic_cast<const CollectableState*>(m_State.get()) != nullptr;
		}

		void SetPlayerBelow();
		void UpdatePlayerBelowTimer(float deltaTime);
		bool WasRecentlyAbovePlayer() const;
	private:
		TileManagerComponent* m_pTileManager{};
		LevelManagerComponent* m_pLevelManager{};

		std::unique_ptr<MoneyBagState> m_State;
		int m_FallDistance = 0;

		bool m_IsMoving = false;
		bool m_HasHitPlayer = false;

		glm::vec3 m_StartPosition{};
		glm::vec3 m_TargetPosition{};
		float m_MoveElapsed = 0.f;
		float m_MoveDuration = 0.25f;

		bool m_WasPlayerBelowRecently = false;
		float m_PlayerBelowTimer = 0.f;
	};
}

