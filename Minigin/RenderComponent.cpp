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

		//can't find a way for my renderComponent to access my transform, I would like feedback on this
		/*auto owner = GetOwner();
		if (owner)
		{
			std::cout << "RenderComponent found its owner: " << owner << std::endl;
			if (owner->HasComponent<Transform>())
			{
				m_transform = owner->GetComponent<Transform>();
				std::cout << "RenderComponent successfully linked to TransformComponent.\n";
			}
			else
			{
				std::cerr << "RenderComponent: No TransformComponent found!\n";
			}
		}
		else
		{
			std::cerr << "RenderComponent: GetOwner() returned nullptr!\n";
		}*/ 
	}

	void dae::RenderComponent::Render() const
	{
		if (!m_texture)
		{
			std::cerr << "Warning: No texture set in RenderComponent!\n";
			return;
		}

		//auto transform = m_transform.lock();
		//if (!transform)
		//{
			//std::cerr << "Warning: RenderComponent is missing a TransformComponent!\n";
			//return;
		//}

		//glm::vec3 position = transform->GetPosition();
		//Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y);
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
