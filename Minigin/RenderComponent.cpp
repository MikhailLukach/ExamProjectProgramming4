#include <iostream>
#include "RenderComponent.h"
#include "Component.h"
#include "GameObject.h"

namespace dae
{
	dae::RenderComponent::RenderComponent(std::shared_ptr<Transform> transform, const std::string& texturePath)
		:m_pTransform(transform)
	{
		if (!texturePath.empty())
		{
			SetTexture(texturePath);
		}
	}

	void dae::RenderComponent::Render() const
	{
		if (!m_texture)
		{
			std::cerr << "Warning: No texture set in RenderComponent!\n";
			return;
		}

		if (auto transform = m_pTransform.lock())
		{
			glm::vec3 position = transform->GetPosition();
			Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y);
		}
		else
		{
			std::cerr << "Warning: TransformComponent is expired!\n";
		}
	}

	void dae::RenderComponent::SetTexture(const std::string& texturePath)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(texturePath);
	}
}
