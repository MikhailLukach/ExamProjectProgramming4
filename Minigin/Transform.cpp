#include "Transform.h"
#include "GameObject.h"


const glm::vec3& dae::Transform::GetWorldPosition()
{
	if (m_transformDirty)
	{
		UpdateWorldPosition();
	}
	return m_worldPosition;
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	m_transformDirty = true;
}

void dae::Transform::SetPosition(const glm::vec3& pos)
{
	m_position = pos;
	m_transformDirty = true;
}

void dae::Transform::SetWorldPosition(const glm::vec3& pos)
{
	if (auto owner = GetOwner()->GetParent())
	{
		m_position = pos - owner->GetWorldPosition();
	}
	else
	{
		m_position = pos;
	}
	m_transformDirty = true;
}

void dae::Transform::Update(float)
{
	if (m_transformDirty)
	{
		UpdateWorldPosition();
	}
}

void dae::Transform::UpdateWorldPosition()
{
	if (auto owner = GetOwner()->GetParent())
	{
		m_worldPosition = owner->GetWorldPosition() + m_position;
	}
	else
	{
		m_worldPosition = m_position;
	}
	m_transformDirty = false;
}
