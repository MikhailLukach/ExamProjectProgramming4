#include "PlayerRespawnComponent.h"
#include "GameObject.h"
#include "InputManager.h"

dae::PlayerRespawnComponent::PlayerRespawnComponent(const glm::vec3& spawnPos)
	:m_SpawnPosition(spawnPos)
{
}

void dae::PlayerRespawnComponent::Notify(EventId event, GameObject* gameObject)
{
	if (event == EventId::PlAYER_HIT && !m_IsRespawning)
	{
		m_IsRespawning = true;
		m_RespawnTimer = 0.f;

		// Hide render
		if (auto render = gameObject->GetComponent<RenderComponent>())
			render->SetVisible(false);

		// Disable input by deactivating GameObject (or better: flag in InputManager)
		dae::InputManager::GetInstance().DisableInputFor(gameObject);
	}
}

void dae::PlayerRespawnComponent::Update(float deltaTime)
{
	if (!m_IsRespawning)
		return;

	m_RespawnTimer += deltaTime;

	if (m_RespawnTimer >= m_RespawnDelay)
	{
		m_IsRespawning = false;

		auto obj = GetOwner();
		obj->GetTransform()->SetPosition(m_SpawnPosition);

		if (auto render = obj->GetComponent<RenderComponent>())
			render->SetVisible(true);

		dae::InputManager::GetInstance().EnableInputFor(obj);
	}
}
