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

/*#pragma once
#include "Component.h"
#include "TileTypes.h"
#include <vector>
#include <memory>

namespace dae
{
	class Scene;
	class TileComponent;

	class LevelLoader : public Component
	{
	public:
		LevelLoader(Scene& scene);
		void Update(float deltaTime) override;

	private:
		void InitLevelLayout();
		void SpawnNextTile();

		Scene& m_Scene;
		std::vector<std::vector<TileVisualType>> m_LevelLayout;

		const int m_LevelWidth = 15;
		const int m_LevelHeight = 10;
		const int m_TileWidth = 42;
		const int m_TileHeight = 43;
		const int m_TopMargin = 48;

		int m_CurrentX = 0;
		int m_CurrentY = 0;
		float m_Timer = 0.0f;
		float m_SpawnInterval = 0.03f; // Delay between tile spawns
	};
}*/

