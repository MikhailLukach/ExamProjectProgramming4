#include "TileManagerComponent.h"

void dae::TileManagerComponent::InitWithTileGrid(std::vector<std::vector<std::shared_ptr<dae::GameObject>>> tileGrid)
{
	m_TileGrid = std::move(tileGrid);
}

std::shared_ptr<dae::GameObject> dae::TileManagerComponent::GetTileAt(int col, int row) const
{
	if (row >= 0 && row < static_cast<int>(m_TileGrid.size()) &&
		col >= 0 && col < static_cast<int>(m_TileGrid[row].size()))
	{
		return m_TileGrid[row][col];
	}
	//std::cout << "[TileManagerComponent] Unable to get tile." << std::endl;
	return nullptr;
}
