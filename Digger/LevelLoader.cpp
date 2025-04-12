#include "LevelLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "TileComponent.h"
#include "RenderComponent.h"
#include "Transform.h"

constexpr int ScreenWidth = 640;
constexpr int ScreenHeight = 480;

constexpr int NumCols = 15;
constexpr int NumRows = 10;

constexpr int TopMargin = 48;

constexpr int TileWidth = ScreenWidth / NumCols;           // 640 / 15 = 42.6 => 42 (int)
constexpr int TileHeight = (ScreenHeight - TopMargin) / NumRows; // (480 - 48) / 10 = 43.2 => 43

void dae::LevelLoader::LoadLevel(Scene& scene)
{
	InitLevelLayout();

	const int LevelPixelWidth = TileWidth * NumCols;
	//const int LevelPixelHeight = TileHeight * NumRows;

	const int HorizontalOffset = (ScreenWidth - LevelPixelWidth) / 2; // ~5
	const int VerticalOffset = TopMargin; // Leave space for UI

	for (int y = 0; y < m_LevelHeight; ++y)
	{
		for(int x = 0; x < m_LevelWidth; ++x)
		{
			auto tile = std::make_shared<dae::GameObject>();

			float posX = static_cast<float>(x * TileWidth + HorizontalOffset);
			float posY = static_cast<float>(y * TileHeight + VerticalOffset);
			tile->GetTransform()->SetPosition(posX, posY, 0.0f);

			TileVisualType type = m_InitialLayout[y][x];
			std::string textureFile = GetTexturePathForType(type);

			auto render = tile->AddComponent<dae::RenderComponent>(textureFile);
			render->SetSize(TileWidth, TileHeight);

			auto logic = tile->AddComponent<TileComponent>(type);
			logic->SetRenderComponent(render.get());

			scene.Add(tile);
		}
	}
}

void dae::LevelLoader::InitLevelLayout()
{
	m_InitialLayout =
	{
		{ TileVisualType::Dug_Bottom, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Corner_BR, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Left },

		{TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Dug_Corner_UR, TileVisualType::Dug_Corner_LB, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Undug, TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Undug, TileVisualType::Dug_Corner_UR, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Corner_LB,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Dug_Vertical,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Dug_Corner_UR,
		TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal,
		TileVisualType::Dug_Corner_LU, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },
	};
}

/*#include "LevelLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "TileComponent.h"
#include "RenderComponent.h"
#include "Transform.h"

dae::LevelLoader::LevelLoader(Scene& scene)
	: m_Scene(scene)
{
	InitLevelLayout();
}

void dae::LevelLoader::Update(float deltaTime)
{
	m_Timer += deltaTime;
	if (m_Timer >= m_SpawnInterval)
	{
		SpawnNextTile();
		m_Timer = 0.0f;
	}
}

void dae::LevelLoader::SpawnNextTile()
{
	if (m_CurrentY >= m_LevelHeight) return;

	auto tile = std::make_shared<GameObject>();

	const int HorizontalOffset = (640 - m_TileWidth * m_LevelWidth) / 2;
	const int VerticalOffset = m_TopMargin;

	float posX = static_cast<float>(m_CurrentX * m_TileWidth + HorizontalOffset);
	float posY = static_cast<float>(m_CurrentY * m_TileHeight + VerticalOffset);

	tile->GetTransform()->SetPosition(posX, posY, 0.0f);

	auto type = m_LevelLayout[m_CurrentY][m_CurrentX];
	std::string texture = GetTexturePathForType(type);

	auto render = tile->AddComponent<RenderComponent>(texture);
	render->SetSize(m_TileWidth, m_TileHeight);

	auto logic = tile->AddComponent<TileComponent>(type);
	logic->SetRenderComponent(render.get());

	m_Scene.Add(tile);

	// Move to next tile
	m_CurrentX++;
	if (m_CurrentX >= m_LevelWidth)
	{
		m_CurrentX = 0;
		m_CurrentY++;
	}
}

void dae::LevelLoader::InitLevelLayout()
{
	m_LevelLayout = {
		{ TileVisualType::Dug_Bottom, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Corner_BR, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Left },

		{TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Dug_Corner_UR, TileVisualType::Dug_Corner_LB, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Undug, TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Undug, TileVisualType::Dug_Corner_UR, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Corner_LB,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Dug_Vertical,
		TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug,
		TileVisualType::Dug_Vertical, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },

		{TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Dug_Corner_UR,
		TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal, TileVisualType::Dug_Horizontal,
		TileVisualType::Dug_Corner_LU, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug, TileVisualType::Undug },
	};
}*/
