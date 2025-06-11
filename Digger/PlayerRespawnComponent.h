#pragma once
#include "Component.h"
#include "Observer.h"
#include <glm.hpp>

namespace dae
{
	class PlayerRespawnComponent : public Component, public Observer
	{
	public:
		PlayerRespawnComponent(const glm::vec3& spawnPos);

		void Update(float deltaTime) override;
		void Notify(EventId event, GameObject* gameObject) override;
	private:
		glm::vec3 m_SpawnPosition;
		bool m_IsRespawning{ false };
		float m_RespawnTimer{ 0.f };
		const float m_RespawnDelay{ 5.0f };;
	};
}

