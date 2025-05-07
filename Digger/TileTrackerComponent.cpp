#include "TileTrackerComponent.h"
#include "GameObject.h"
#include "Transform.h"

dae::TileTrackerComponent::TileTrackerComponent(int tileWidth, int tileHeight, int offsetX, int offsetY)
	:m_TileWidth(tileWidth), m_TileHeight(tileHeight), m_OffsetX(offsetX), m_OffsetY(offsetY)
{
}

void dae::TileTrackerComponent::Update(float deltaTime)
{
	(void)deltaTime;
	if (!GetOwner()) return;

	auto pos = GetOwner()->GetTransform()->GetWorldPosition();

	int col = static_cast<int>((pos.x - m_OffsetX) / m_TileWidth);
	int row = static_cast<int>((pos.y - m_OffsetY) / m_TileHeight);

	m_CurrentTile = { col, row };

	std::cout << "[TileTrackerComponent]: The current tile is y: " << col << " and x: " << row << std::endl;
}
