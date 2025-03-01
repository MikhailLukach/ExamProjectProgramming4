#pragma once
#include <glm.hpp>
#include "Component.h"

namespace dae
{
	class Transform : public Component
	{
	public:
		const glm::vec3& GetPosition() const { return m_position; }
		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition();
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& pos);
		void SetWorldPosition(const glm::vec3& pos);

		void Update(float deltaTime) override;
	private:
		glm::vec3 m_position;
		glm::vec3 m_worldPosition{ 0.0f, 0.0f, 0.0f };

		bool m_transformDirty = true;
		void UpdateWorldPosition();
	};
}
