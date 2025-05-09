#pragma once
#include "TileTypes.h"
#include <vector>
#include <memory>
#include <glm.hpp>

namespace dae
{
	class Scene;
	class GameObject;

	class LevelLoader
	{
	public:
		LevelLoader() = default;
		void LoadLevel(Scene& scene, std::vector<std::vector<std::shared_ptr<dae::GameObject>>>& outTileGrid);
		glm::vec3 GetWorldCenterForTile(int tileX, int tileY) const;
	private:
		const int m_LevelWidth = 15;
		const int m_LevelHeight = 10;

		std::vector<std::vector<TileVisualType>> m_InitialLayout;

		void InitLevelLayout();
	};
}


