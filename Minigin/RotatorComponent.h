#pragma once
#include "Component.h"
#include <glm.hpp>
#include <memory>

namespace dae
{
	class Transform;
	class RotatorComponent : public Component
	{
	public:
		RotatorComponent( float radius, float speed, const glm::vec3& center);
		void Update(float deltaTime) override;
		//void SetCenter(const glm::vec3& newCenter);
		void SetRadius(float radius);
		void SetSpeed(float speed);
	private:
		glm::vec3 m_center;
		float m_radius;
		float m_speed;
		float m_angle;
	};
}

