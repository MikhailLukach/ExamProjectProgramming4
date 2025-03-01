#include <iostream>
#include "RenderComponent.h"
#include "Component.h"
#include "GameObject.h"

namespace dae
{
	dae::RenderComponent::RenderComponent(const std::string& texturePath)
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

		glm::vec3 position = GetOwner()->GetTransform()->GetLocalPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y);
	}

	void dae::RenderComponent::SetTexture(const std::string& texturePath)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(texturePath);
	}
}
