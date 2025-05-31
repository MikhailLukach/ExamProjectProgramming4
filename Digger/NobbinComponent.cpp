#include "NobbinComponent.h"
#include "Transform.h"
#include "GameObject.h"
#include <iostream>

void dae::NobbinComponent::Update(float deltaTime)
{
	if (!m_IsMoving) return;

	m_MoveElapsed += deltaTime;
	float t = std::min(m_MoveElapsed / m_MoveDuration, 1.f);
	glm::vec3 newPos = glm::mix(m_StartPosition, m_TargetPosition, t);
	GetOwner()->GetTransform()->SetWorldPosition(newPos);

	if (t >= 1.f)
	{
		m_IsMoving = false;
		std::cout << "[Nobbin] Move complete to tile.\n";
	}
}

void dae::NobbinComponent::NobbinStartMoveTo(const glm::vec3& targetPos, float duration)
{
	m_StartPosition = GetOwner()->GetTransform()->GetWorldPosition();
	m_TargetPosition = targetPos;
	m_MoveElapsed = 0.f;
	m_MoveDuration = duration;
	m_IsMoving = true;

	std::cout << "[Nobbin] Begin smooth move to ("
		<< targetPos.x << ", " << targetPos.y << ")\n";
}

bool dae::NobbinComponent::NobbinIsMoving() const
{
	return m_IsMoving;
}
