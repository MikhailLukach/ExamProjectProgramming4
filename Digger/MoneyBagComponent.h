#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
	class MoneyBagState;
	class MoneyBagComponent : public Component
	{
	public:
		MoneyBagComponent() = default;
		void Update(float deltaTime) override;
		void SetState(std::unique_ptr<MoneyBagState> newState);

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
	};
}

