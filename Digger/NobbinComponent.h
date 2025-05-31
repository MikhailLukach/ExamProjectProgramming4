#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae
{
	class NobbinComponent : public Component
	{
	public:
		void Update(float deltaTime) override;
		
		void NobbinStartMoveTo(const glm::vec3& targetPos, float duration = 0.25f);
		bool NobbinIsMoving() const;
	private:
		bool m_IsMoving = false;
		glm::vec3 m_StartPosition{};
		glm::vec3 m_TargetPosition{};
		float m_MoveElapsed = 0.f;
		float m_MoveDuration = 0.25f;
	};
}

