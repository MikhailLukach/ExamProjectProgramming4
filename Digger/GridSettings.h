#pragma once

namespace dae
{
	struct GridSettings
	{
		inline static constexpr int TileWidth = 42;
		inline static constexpr int TileHeight = 43;
		inline static constexpr int OffsetX = (640 - TileWidth * 15) / 2;
		inline static constexpr int OffsetY = 48;

		inline static constexpr int GridOffsetX = (OffsetX - TileWidth / 2);
		inline static constexpr int GridOffsetY = (OffsetY - TileHeight / 2);

		inline static constexpr int NumCols = 15;
		inline static constexpr int NumRows = 10;
	};
}