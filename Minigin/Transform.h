#pragma once
#include <glm.hpp>
#include "Component.h"

namespace dae
{
	class Transform : public Component
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);

		void Update([[maybe_unused]] float deltaTime);
	private:
		glm::vec3 m_position;
	};
}
