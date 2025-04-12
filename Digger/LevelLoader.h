#pragma once
#include "TileTypes.h"
#include <vector>
#include <memory>

namespace dae
{
	class Scene;
	class GameObject;

	class LevelLoader
	{
	public:
		LevelLoader() = default;
		void LoadLevel(Scene& scene);
	private:
		const int m_LevelWidth = 15;
		const int m_LevelHeight = 10;

		std::vector<std::vector<TileVisualType>> m_InitialLayout;

		void InitLevelLayout();
	};
}

