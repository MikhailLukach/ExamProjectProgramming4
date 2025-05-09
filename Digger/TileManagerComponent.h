#pragma once
#include "Component.h"
#include <vector>

namespace dae
{
	class TileManagerComponent : public Component
	{
	public:
		void InitWithTileGrid(std::vector<std::vector<std::shared_ptr<dae::GameObject>>> tileGrid);

		std::shared_ptr<dae::GameObject> GetTileAt(int col, int row) const;

	private:
		std::vector<std::vector<std::shared_ptr<dae::GameObject>>> m_TileGrid;
	};
}

