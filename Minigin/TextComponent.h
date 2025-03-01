#pragma once
#include "Component.h"
#include "Font.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Transform.h"
#include <string>
#include <memory>

namespace dae
{
	class TextComponent : public Component
	{
	public:
		TextComponent(const std::string& text, const std::string& fontPath, unsigned int fontSize);
		void Render() const override;
		void SetText(const std::string& newText);
		void SetColor(SDL_Color color);
		//void SetPosition(float x, float y);
	private:
		void UpdateTexture();

		std::string m_text;
		SDL_Color m_color = { 255, 255, 255, 255 };
		std::shared_ptr<Font> m_font;
		std::shared_ptr<Texture2D> m_texture;

		//float m_x = 0.0f, m_y = 0.0f;
	};
}