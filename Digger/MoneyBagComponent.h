#pragma once
#include "Component.h"
#include <memory>
#include <glm.hpp>

namespace dae
{
	class MoneyBagState;
	class TileManagerComponent;
	class MoneyBagComponent : public Component
	{
	public:
		MoneyBagComponent() = default;
		void Update(float deltaTime) override;
		void SetState(std::unique_ptr<MoneyBagState> newState);

		void TryPushHorizontally(int direction, TileManagerComponent* tileManager);

		void StartMoveTo(const glm::vec3& targetPos);

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
	private:
		std::unique_ptr<MoneyBagState> m_State;
		int m_FallDistance = 0;

		bool m_IsMoving = false;
		glm::vec3 m_StartPosition{};
		glm::vec3 m_TargetPosition{};
		float m_MoveElapsed = 0.f;
		float m_MoveDuration = 0.25f;
	};
}

