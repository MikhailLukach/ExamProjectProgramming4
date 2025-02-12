#include <SDL.h>
#include <SDL_image.h>
#include "Texture2D.h"
#include "Renderer.h"
#include <stdexcept>
#include <iostream>

dae::Texture2D::~Texture2D()
{
	SDL_DestroyTexture(m_texture);
}

glm::ivec2 dae::Texture2D::GetSize() const
{
	SDL_Rect dst;
	SDL_QueryTexture(GetSDLTexture(), nullptr, nullptr, &dst.w, &dst.h);
	return { dst.w,dst.h };
}

SDL_Texture* dae::Texture2D::GetSDLTexture() const
{
	return m_texture;
}

dae::Texture2D::Texture2D(const std::string &fullPath)
{
	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();
	if (!renderer)
	{
		throw std::runtime_error("Error: Renderer is not initialized before loading texture: " + fullPath);
	}

	std::cout << "Loading texture from: " << fullPath << std::endl;

	m_texture = IMG_LoadTexture(renderer, fullPath.c_str());
	if (!m_texture)
	{
		std::cerr << "Error loading texture: " << fullPath << " - " << IMG_GetError() << std::endl;
		throw std::runtime_error(std::string("Failed to load texture: ") + IMG_GetError());
	}
	//m_texture = IMG_LoadTexture(Renderer::GetInstance().GetSDLRenderer(), fullPath.c_str());
	//if (m_texture == nullptr)
		//throw std::runtime_error(std::string("Failed to load texture: ") + SDL_GetError());
}

dae::Texture2D::Texture2D(SDL_Texture* texture)	: m_texture{ texture } 
{
	assert(m_texture != nullptr);
}

