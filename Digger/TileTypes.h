#pragma once
#include <cstdint>
#include <string>

namespace dae
{
	enum class TileVisualType
	{
		Undug,
		Dug_Spot,
		Dug_Bottom,
		Dug_Up,
		Dug_Left,
		Dug_Right,
		Dug_Corner_BR,
		Dug_Corner_LB,
		Dug_Corner_LU,
		Dug_Corner_UR,
		Dug_TCross_LBR,
		Dug_TCross_LUR,
		Dug_TCross_ULB,
		Dug_TCRoss_URB,
		Dug_Vertical,
		Dug_Horizontal,
		Dug_Cross
	};

    inline std::string GetTexturePathForType(TileVisualType type)
    {
        switch (type)
        {
        case TileVisualType::Undug:
            return "Tiles/TileFilled.png";
        case TileVisualType::Dug_Spot:
            return "Tiles/TileBlack.png";
        default:
            return "error.png";
        }
    }
}
