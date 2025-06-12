#pragma once
#include "Component.h"
#include "Observer.h"
#include <glm.hpp>

namespace dae
{
	class LevelManagerComponent;
	class PlayerRespawnComponent : public Component, public Observer
	{
	public:
		PlayerRespawnComponent(const glm::vec3& spawnPos, LevelManagerComponent* levelManager = nullptr);

		void Update(float deltaTime) override;
		void Notify(EventId event, GameObject* gameObject) override;
	private:
		glm::vec3 m_SpawnPosition;

		LevelManagerComponent* m_pLevelManager;

		bool m_IsRespawning{ false };
		bool m_HasDied{ false };
		float m_RespawnTimer{ 0.f };
		const float m_RespawnDelay{ 5.0f };;
	};
}

