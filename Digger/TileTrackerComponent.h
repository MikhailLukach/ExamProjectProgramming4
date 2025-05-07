#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae
{
	class TileTrackerComponent : public Component
	{
	public:
		TileTrackerComponent(int tileWidth, int tileHeight, int offsetX, int offsetY);

		void Update(float deltaTime) override;

		glm::ivec2 GetTileCoords() const { return m_CurrentTile; }
	private:
		int m_TileWidth{};
		int m_TileHeight{};
		int m_OffsetX{};
		int m_OffsetY{};
		glm::ivec2 m_CurrentTile{};
	};
}

