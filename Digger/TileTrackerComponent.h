#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae
{
	enum class TrackingMode
	{
		Center,
		TopLeft
	};
	class TileTrackerComponent : public Component
	{
	public:
		TileTrackerComponent(int tileWidth, int tileHeight, int offsetX, int offsetY);

		void Update(float deltaTime) override;

		glm::ivec2 GetTileCoords() const { return m_CurrentTile; }
		glm::ivec2 GetCenterTileCoords() const;

		void SetTrackingMode(TrackingMode mode) { m_TrackingMode = mode; }
	private:
		int m_TileWidth{};
		int m_TileHeight{};
		int m_OffsetX{};
		int m_OffsetY{};
		glm::ivec2 m_CurrentTile{};
		TrackingMode m_TrackingMode = TrackingMode::Center;
	};
}

