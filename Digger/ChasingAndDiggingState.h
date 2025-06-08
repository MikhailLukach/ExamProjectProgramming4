#pragma once
#include "NobbinState.h"
#include <glm.hpp>

namespace dae
{
	class ChasingAndDiggingState : public NobbinState
	{
	public:
		void OnEnter(NobbinControllerComponent& controller) override;
		void OnExit(NobbinControllerComponent& controller) override;

		void Update(NobbinControllerComponent& controller, float deltaTime) override;

		const char* GetID() const override { return "ChasingAndDigging"; }
	private:
		glm::ivec2 m_LastMoveDir{};
	};
}

