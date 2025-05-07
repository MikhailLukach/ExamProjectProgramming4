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
		/*if (!m_texture)
		{
			std::cerr << "Warning: No texture set in RenderComponent!\n";
			return;
		}

		glm::vec3 position = GetOwner()->GetTransform()->GetLocalPosition();
		Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y, m_Width, m_Height);*/

		if (!m_texture)
		{
			std::cerr << "Warning: No texture set in RenderComponent!\n";
			return;
		}

		glm::vec3 position = GetOwner()->GetTransform()->GetLocalPosition();

		SDL_Rect dst{};
		dst.x = static_cast<int>(position.x + m_RenderOffset.x);
		dst.y = static_cast<int>(position.y + m_RenderOffset.y);
		dst.w = static_cast<int>(m_Width);
		dst.h = static_cast<int>(m_Height);

		SDL_Renderer* sdlRenderer = Renderer::GetInstance().GetSDLRenderer();

		if (m_UseSourceRect)
		{
			SDL_RenderCopy(sdlRenderer, m_texture->GetSDLTexture(), &m_SourceRect, &dst);
		}
		else
		{
			SDL_RenderCopy(sdlRenderer, m_texture->GetSDLTexture(), nullptr, &dst);
		}
	}

	void dae::RenderComponent::SetTexture(const std::string& texturePath)
	{
		m_texture = ResourceManager::GetInstance().LoadTexture(texturePath);
	}

	void dae::RenderComponent::SetSize(int width, int height)
	{
		m_Width = static_cast<float>(width);
		m_Height = static_cast<float>(height);
	}

	void RenderComponent::SetSourceRect(const SDL_Rect& rect)
	{
		m_SourceRect = rect;
		m_UseSourceRect = true;
	}

	void RenderComponent::ClearSourceRect()
	{
		m_UseSourceRect = false;
	}

	void RenderComponent::SetRenderOffset(const glm::vec2& offset)
	{
		m_RenderOffset = offset;
	}

	void RenderComponent::ResetRenderOffset()
	{
		m_RenderOffset = { 0.f, 0.f };
	}
}
