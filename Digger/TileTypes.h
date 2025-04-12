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
            return "Tiles/TileEmpty.png";
        case TileVisualType::Dug_Bottom:
            return "Tiles/TileBottom.png";
        case TileVisualType::Dug_Up:
            return "Tiles/TileUp.png";
        case TileVisualType::Dug_Left:
            return "Tiles/TileLeft.png";
        case TileVisualType::Dug_Right:
            return "Tiles/TileRight.png";
        case TileVisualType::Dug_Corner_BR:
            return "Tiles/TileCornerBR.png";
        case TileVisualType::Dug_Corner_LB:
            return "Tiles/TileCornerLB.png";
        case TileVisualType::Dug_Corner_LU:
            return "Tiles/TileCornerLU.png";
        case TileVisualType::Dug_Corner_UR:
            return "Tiles/TileCornerUR.png";
        case TileVisualType::Dug_TCross_LBR:
            return "Tiles/TileTCrossLBR.png";
        case TileVisualType::Dug_TCross_LUR:
            return "Tiles/TileTCrossLUR.png";
        case TileVisualType::Dug_TCross_ULB:
            return "Tiles/TileTCrossULB.png";
        case TileVisualType::Dug_TCRoss_URB:
            return "Tiles/TileTCrossURB.png";
        case TileVisualType::Dug_Vertical:
            return "Tiles/TileVertical.png";
        case TileVisualType::Dug_Horizontal:
            return "Tiles/TileHorizontal.png";
        case TileVisualType::Dug_Cross:
            return "Tiles/TileBlack.png";
        default:
            return "error.png";
        }
    }
}
