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
		Renderer::GetInstance().RenderTexture(*m_texture, m_x, m_y);
	}

	void dae::RenderComponent::SetTexture(const std::string& texturePath)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(texturePath);
	}

	void dae::RenderComponent::SetPosition(float x, float y)
	{
		m_x = x;
		m_y = y;
	}
}
