#include "MoneyBagComponent.h"
#include "TileTrackerComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include "TileManagerComponent.h"
#include "MoneyBagState.h"
#include <iostream>

void dae::MoneyBagComponent::Update(float deltaTime)
{
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

void dae::MoneyBagComponent::StartMoveTo(const glm::vec3& targetPos)
{
	m_StartPosition = GetOwner()->GetTransform()->GetWorldPosition();
	m_TargetPosition = targetPos;
	m_MoveElapsed = 0.f;
	m_IsMoving = true;

	std::cout << "[MoneyBag] Begin smooth move to target.\n";
}
