#include "TextComponent.h"
#include "GameObject.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

dae::TextComponent::TextComponent(std::shared_ptr<Transform> transform, const std::string& text, const std::string& fontPath, unsigned int fontSize)
    :m_pTransform(transform), m_text(text)
{
	m_font = ResourceManager::GetInstance().LoadFont(fontPath, static_cast<uint8_t>(fontSize));
	UpdateTexture();
}

void dae::TextComponent::Render() const
{
    if (!m_texture)
    {
        std::cerr << "TextComponent: No texture to render!\n";
        return;
    }

    if (auto transform = m_pTransform.lock())
    {
        glm::vec3 position = transform->GetLocalPosition();
        Renderer::GetInstance().RenderTexture(*m_texture, position.x, position.y);
    }
    else
    {
        std::cerr << "TextComponent: TransformComponent is expired!\n";
    }
}

void dae::TextComponent::SetText(const std::string& newText)
{
    if (m_text == newText)
    {
        return;
    }
    m_text = newText;
    UpdateTexture();
}

void dae::TextComponent::SetColor(SDL_Color color)
{
    m_color = color;
    UpdateTexture();
}

/*void dae::TextComponent::SetPosition(float x, float y)
{
    m_x = x;
    m_y = y;
}*/

void dae::TextComponent::UpdateTexture()
{
    if (!m_font)
    {
        std::cerr << "TextComponent: No valid font found!\n";
        return;
    }

    SDL_Surface* surface = TTF_RenderText_Blended(m_font->GetFont(), m_text.c_str(), m_color);
    if (!surface)
    {
        std::cerr << "TextComponent: Failed to render text - " << TTF_GetError() << "\n";
        return;
    }

    SDL_Texture* sdlTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), surface);
    SDL_FreeSurface(surface);

    if (!sdlTexture)
    {
        std::cerr << "TextComponent: Failed to create texture from rendered text - " << SDL_GetError() << "\n";
        return;
    }

    m_texture = std::make_shared<Texture2D>(sdlTexture);
}
