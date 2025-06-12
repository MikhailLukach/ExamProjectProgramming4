#pragma once
#include "Component.h"
#include <vector>
#include <glm.hpp>

namespace dae
{
	class TileManagerComponent : public Component
	{
	public:
		void InitWithTileGrid(std::vector<std::vector<std::shared_ptr<dae::GameObject>>> tileGrid);

		std::shared_ptr<dae::GameObject> GetTileAt(int col, int row) const;

		bool IsDugTile(const glm::ivec2& tile) const;

	private:
		std::vector<std::vector<std::shared_ptr<dae::GameObject>>> m_TileGrid;
	};
}

