#pragma once
#include "Component.h"
#include <SDL.h>

namespace dae
{
	class GridOutlineComponent : public Component
	{
	public:
		GridOutlineComponent(int tileWidth, int tileHeight, SDL_Color color = { 0, 255, 0, 255 });

		void Render() const override;

	private:
		int m_TileWidth{};
		int m_TileHeight{};
		SDL_Color m_Color{};
	};
}

