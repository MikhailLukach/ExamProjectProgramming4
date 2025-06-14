#pragma once
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"
#include <memory>
//both for render and text component, pass a transform component and take the location from it
namespace dae
{
	class RenderComponent : public Component
	{
	public:
		explicit RenderComponent(const std::string& texturePath = "");
		void Render() const override;
		void SetTexture(const std::string& texturePath);
		void SetSize(int width, int height);

		void SetSourceRect(const SDL_Rect& rect);
		void ClearSourceRect();

		void SetRenderOffset(const glm::vec2& offset);
		void ResetRenderOffset();

		void SetVisible(bool visible) { m_IsVisible = visible; }
		bool IsVisible() const { return m_IsVisible; }

		glm::vec2 GetRenderOffset() const { return m_RenderOffset; }
		float GetWidth() const { return m_Width; }
		float GetHeight() const { return m_Height; }
	private:
		std::shared_ptr<Texture2D> m_texture;

		SDL_Rect m_SourceRect{};
		bool m_UseSourceRect{ false };

		bool m_IsVisible{ true };

		//std::weak_ptr<Transform> m_pTransform;
		float m_x{ 0 }, m_y{ 0 }, m_Width{ 0 }, m_Height{ 0 };

		glm::vec2 m_RenderOffset{ 0.f, 0.f };
	};
}


