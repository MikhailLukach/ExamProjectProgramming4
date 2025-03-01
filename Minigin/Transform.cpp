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

const glm::vec3& dae::Transform::GetLocalPosition()
{
	return m_position;
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
	MarkTransformDirty();
	//m_transformDirty = true;
}

void dae::Transform::SetPosition(const glm::vec3& pos)
{
	m_position = pos;
	MarkTransformDirty();
	//m_transformDirty = true;
}

void dae::Transform::SetWorldPosition(const glm::vec3& pos)
{
	if (auto owner = GetOwner()->GetParent())
	{
		m_position = pos - owner->GetTransform()->GetWorldPosition();
	}
	else
	{
		m_position = pos;
	}
	MarkTransformDirty();
	//m_transformDirty = true;
}

void dae::Transform::Update(float)
{
	if (m_transformDirty)
	{
		UpdateWorldPosition();
	}
}

void dae::Transform::MarkTransformDirty()
{
	if (!m_transformDirty)
	{
		m_transformDirty = true;

		auto owner = GetOwner();
		for (size_t i = 0; i < owner->GetChildCount(); ++i)
		{
			owner->GetChildAt(i)->GetTransform()->MarkTransformDirty();
		}
	}
}

void dae::Transform::UpdateWorldPosition()
{
	if (auto owner = GetOwner()->GetParent())
	{
		m_worldPosition = owner->GetTransform()->GetWorldPosition() + m_position;
	}
	else
	{
		m_worldPosition = m_position;
	}
	m_transformDirty = false;
}
