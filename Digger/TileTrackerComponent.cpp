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

    float x = pos.x;
    float y = pos.y;

    if (m_TrackingMode == TrackingMode::Center)
    {
        x += 16.0f;
        y += 16.0f;
    }

    int col = static_cast<int>((x - m_OffsetX) / m_TileWidth);
    int row = static_cast<int>((y - m_OffsetY) / m_TileHeight);

    m_CurrentTile = { col, row };

	//std::cout << "[TileTrackerComponent]: The current tile is y: " << col << " and x: " << row << std::endl;
}

glm::ivec2 dae::TileTrackerComponent::GetCenterTileCoords() const
{
    if (!GetOwner()) return { -1, -1 };
    auto pos = GetOwner()->GetTransform()->GetWorldPosition();
    float centerX = pos.x + 16.0f;
    float centerY = pos.y + 16.0f;

    int col = static_cast<int>((centerX - m_OffsetX) / m_TileWidth);
    int row = static_cast<int>((centerY - m_OffsetY) / m_TileHeight);
    return { col, row };
}
