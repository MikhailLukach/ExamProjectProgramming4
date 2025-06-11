#include "MoneyBagComponent.h"
#include "TileTrackerComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "TileManagerComponent.h"
#include "MoneyBagState.h"
#include "SceneManager.h"
#include "Scene.h"
#include <iostream>
#include "LivesComponent.h"

dae::MoneyBagComponent::MoneyBagComponent(LevelManagerComponent* levelManager)
	:m_pLevelManager(levelManager)
{
}

void dae::MoneyBagComponent::Render() const
{
#ifdef _DEBUG // Optional: only in debug builds
	SDL_Renderer* renderer = dae::Renderer::GetInstance().GetSDLRenderer(); // Or your equivalent

	if (renderer)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Red for falling bag

		int boxSize = 16; // Match your sprite/tile size
		auto pos = GetOwner()->GetTransform()->GetWorldPosition();

		SDL_Rect debugRect{};
		debugRect.x = static_cast<int>(pos.x - boxSize / 2);
		debugRect.y = static_cast<int>(pos.y - boxSize / 2);
		debugRect.w = boxSize;
		debugRect.h = boxSize;

		SDL_RenderDrawRect(renderer, &debugRect);
	}
#endif
}

void dae::MoneyBagComponent::Update(float deltaTime)
{
	UpdatePlayerBelowTimer(deltaTime);
	if (!m_IsMoving && m_State)
	{
		std::unique_ptr<MoneyBagState> newState = m_State->Update(*this, deltaTime);
		if (newState)
		{
			SetState(std::move(newState));
		}
	}

	if (!m_IsMoving) return;

	m_MoveElapsed += deltaTime;
	float t = std::min(m_MoveElapsed / m_MoveDuration, 1.f);

	glm::vec3 newPos = glm::mix(m_StartPosition, m_TargetPosition, t);
	GetOwner()->GetTransform()->SetPosition(newPos);

	if (t >= 1.f)
	{
		m_IsMoving = false;
		std::cout << "[MoneyBag] Move complete\n";
	}

	if (IsFalling() && !m_HasHitPlayer)
	{
		auto bagPos = GetOwner()->GetTransform()->GetWorldPosition();

		auto* scene = dae::SceneManager::GetInstance().GetCurrentScene();
		if (scene)
		{
			for (auto& obj : scene->GetObjects())
			{
				if (!obj || obj.get() == GetOwner()) continue;

				auto lives = obj->GetComponent<LivesComponent>();
				if (!lives) continue;

				auto playerPos = obj->GetTransform()->GetWorldPosition();
				float overlapThreshold = 16.0f;

				if (std::abs(playerPos.x - bagPos.x) < overlapThreshold &&
					std::abs(playerPos.y - bagPos.y) < overlapThreshold)
				{
					std::cout << "[MoneyBag] Player hit mid-fall!\n";
					lives->LoseLife();
					m_HasHitPlayer = true;
					// Optional: apply crushed behavior, animation, etc.
					break;
				}
			}
		}
	}
}

void dae::MoneyBagComponent::SetState(std::unique_ptr<MoneyBagState> newState)
{
	if (m_State)
	{
		m_State->OnExit(*this);
	}

	m_State = std::move(newState);

	if (m_State)
	{
		m_State->OnEnter(*this);
	}
}

void dae::MoneyBagComponent::TryPushHorizontally(int direction, TileManagerComponent* tileManager)
{
	if (m_State)
	{
		m_State->TryPushHorizontally(*this, direction, tileManager);
	}
}

void dae::MoneyBagComponent::StartMoveTo(const glm::vec3& targetPos, float duration)
{
	m_StartPosition = GetOwner()->GetTransform()->GetWorldPosition();
	m_TargetPosition = targetPos;
	m_MoveElapsed = 0.f;
	m_MoveDuration = duration;
	m_IsMoving = true;

	std::cout << "[MoneyBag] Begin smooth move to target.\n";
}

void dae::MoneyBagComponent::SetPlayerBelow()
{
	m_WasPlayerBelowRecently = true;
	m_PlayerBelowTimer = 2.f;
}

void dae::MoneyBagComponent::UpdatePlayerBelowTimer(float deltaTime)
{
	if (m_WasPlayerBelowRecently)
	{
		m_PlayerBelowTimer -= deltaTime;
		if (m_PlayerBelowTimer <= 0.f)
		{
			m_WasPlayerBelowRecently = false;
		}
	}
}

bool dae::MoneyBagComponent::WasRecentlyAbovePlayer() const
{
	return m_WasPlayerBelowRecently;
}
