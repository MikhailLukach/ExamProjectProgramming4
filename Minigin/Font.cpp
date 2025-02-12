#include <stdexcept>
#include "Font.h"

TTF_Font* dae::Font::GetFont() const {
	return m_font;
}

dae::Font::Font(const std::string& fullPath, unsigned int size) : m_font(nullptr)
{
	m_font = TTF_OpenFont(fullPath.c_str(), size);
	if (m_font == nullptr) 
	{
		throw std::runtime_error("Failed to load font: " + fullPath + " - " + TTF_GetError());
	}
}

dae::Font::~Font()
{
	if (m_font)
	{
		TTF_CloseFont(m_font);
	}
}
