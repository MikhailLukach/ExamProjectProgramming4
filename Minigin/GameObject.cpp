#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
dae::GameObject::GameObject()
{
	m_pTransform = std::make_shared<Transform>();
	m_pTransform->SetOwner(this);
}

//Don't assume that gameobject doesn't have a transform, it will always have a transform
//otherwise too many if checks
dae::GameObject::~GameObject() = default;

void dae::GameObject::Update(float deltaTime)
{
	for (auto& [type, component] : m_components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	for (const auto& [type, component] : m_components)
	{
		component->Render();
	}
}

std::shared_ptr<dae::GameObject> dae::GameObject::GetParent() const
{
	if (m_pParent.expired())
	{
		return nullptr;
	}
	return m_pParent.lock();
}

void dae::GameObject::SetParent(std::shared_ptr<GameObject> newParent, bool keepWorldPosition)
{
	if (IsChild(std::const_pointer_cast<const GameObject>(newParent)) || newParent.get() == this
		|| m_pParent.lock() == newParent)
	{
		return;
	}

	//auto transform = GetComponent<Transform>();

	if (!newParent)
	{
		m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition()); // Keep world position
	}
	else
	{
		if (keepWorldPosition)
		{
			m_pTransform->SetPosition(m_pTransform->GetWorldPosition() - newParent->GetTransform()->GetWorldPosition());
		}
	}

	if (auto currentParent = m_pParent.lock())
	{
		auto& siblings = currentParent->m_pVecChildren;
		siblings.erase(std::remove(siblings.begin(), siblings.end(), shared_from_this()), siblings.end());
	}

	m_pParent = newParent;

	if (m_pParent.lock())
	{
		m_pParent.lock()->m_pVecChildren.push_back(shared_from_this());
	}
}

bool dae::GameObject::IsChild(std::shared_ptr<const GameObject> potentialParent)
{
	if (!potentialParent)
	{
		return false;
	}

	for (const auto& child : potentialParent->m_pVecChildren)
	{
		if (child == std::const_pointer_cast<GameObject>(shared_from_this()) 
			|| child->IsChild(std::const_pointer_cast<GameObject>(shared_from_this())))
		{
			return true;
		}
	}
	return false;
}

std::shared_ptr<dae::GameObject> dae::GameObject::GetChildAt(size_t index) const
{
	if (index >= m_pVecChildren.size())
	{
		return nullptr;
	}
	return m_pVecChildren[index];
}

void dae::GameObject::RemoveChild(std::shared_ptr<GameObject> child)
{
	if (!child || child->m_pParent.lock().get() != this)
	{
		return;
	}

	m_pVecChildren.erase(std::remove(m_pVecChildren.begin(), m_pVecChildren.end(), child), m_pVecChildren.end());

	child->m_pParent.reset();
}

void dae::GameObject::MarkTransformDirty()
{
	if (!m_transformDirty)
	{
		m_transformDirty = true;
		for (auto& child : m_pVecChildren)
		{
			child->MarkTransformDirty();
		}
	}
}
