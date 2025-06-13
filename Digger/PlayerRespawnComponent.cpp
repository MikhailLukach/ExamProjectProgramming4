#include "PlayerRespawnComponent.h"
#include "LevelManagerComponent.h"
#include "GameObject.h"
#include "InputManager.h"
#include <SoundServiceLocator.h>



void dae::PlayerRespawnComponent::Notify(EventId event, GameObject* gameObject)
{
	if (event == EventId::PlAYER_HIT && !m_IsRespawning)
	{
		m_IsRespawning = true;
		m_RespawnTimer = 0.f;

		if (auto render = gameObject->GetComponent<RenderComponent>())
			render->SetVisible(false);

		dae::InputManager::GetInstance().DisableInputFor(gameObject);
	}
	else if (event == EventId::PLAYER_DIED)
	{
		if (auto render = gameObject->GetComponent<RenderComponent>())
			render->SetVisible(false);

		dae::InputManager::GetInstance().DisableInputFor(gameObject);

		m_HasDied = true;
		if (m_pLevelManager)
		{
			m_pLevelManager->UnregisterPlayer(gameObject);
		}
		gameObject->MarkForDeletion();
	}
}

dae::PlayerRespawnComponent::PlayerRespawnComponent(const glm::vec3& spawnPos, LevelManagerComponent* levelManager)
	:m_SpawnPosition(spawnPos)
	, m_pLevelManager(levelManager)
{
}

void dae::PlayerRespawnComponent::Update(float deltaTime)
{
	if (!m_IsRespawning || m_HasDied)
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
		dae::SoundServiceLocator::Get().PlayMusic(dae::ResourceManager::GetInstance().GetFullPath("02LevelBGMLoopable.wav"), -1);
	}
}
