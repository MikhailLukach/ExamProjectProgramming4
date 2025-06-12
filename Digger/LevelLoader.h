#pragma once
#include "TileTypes.h"
#include <vector>
#include <memory>
#include <glm.hpp>

namespace dae
{
	class Scene;
	class GameObject;
	class TileManagerComponent;
	class LevelManagerComponent;

	struct LevelData 
	{
		uint16_t width{};
		uint16_t height{};
		std::vector<uint8_t> tiles;           // size = width*height
		std::vector<glm::ivec2> moneyPos;
		std::vector<glm::ivec2> gemPos;
	};

	class LevelLoader
	{
	public:
		LevelLoader() = default;
		void LoadLevelFromFile(const std::string& levelFile);
		bool LoadFromBinary(const std::string& filename, LevelData& out);
		void LoadLevel(Scene& scene, std::vector<std::vector<std::shared_ptr<dae::GameObject>>>& outTileGrid);
		void LoadLevelBinary(
			const std::string& filename,
			Scene& scene,
			std::vector<std::vector<std::shared_ptr<GameObject>>>& outTileGrid,
			LevelManagerComponent* lvlMgr,
			TileManagerComponent* tileMgr);
		glm::vec3 GetWorldCenterForTile(int tileX, int tileY) const;
	private:
		const int m_LevelWidth = 15;
		const int m_LevelHeight = 10;

		std::vector<std::vector<TileVisualType>> m_InitialLayout;
	};
}


