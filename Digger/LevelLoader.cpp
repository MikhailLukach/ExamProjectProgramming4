#include "LevelLoader.h"
#include "Scene.h"
#include "GameObject.h"
#include "TileComponent.h"
#include "RenderComponent.h"
#include "GridOutlineComponent.h"
#include "TileManagerComponent.h"
#include "LevelManagerComponent.h"
#include "Transform.h"
#include <fstream>
#include <sstream> 
#include "GameObjectFactory.h"
#include "GridSettings.h"

constexpr int ScreenWidth = 640;
constexpr int ScreenHeight = 480;

constexpr int NumCols = 15;
constexpr int NumRows = 10;

constexpr int TopMargin = 48;
constexpr int HorOffset = 10;

constexpr int TileWidth = ScreenWidth / NumCols;           // 640 / 15 = 42.6 => 42 (int)
constexpr int TileHeight = (ScreenHeight - TopMargin) / NumRows; // (480 - 48) / 10 = 43.2 => 43

void dae::LevelLoader::LoadLevelFromFile(const std::string& levelFile)
{
	std::string fullPath = ResourceManager::GetInstance().GetFullPath(levelFile);
	std::ifstream file(fullPath);

	if (!file.is_open())
	{
		std::cerr << "[LevelLoader] Failed to open level file: " << fullPath << std::endl;
		return;
	}

	m_InitialLayout.clear();
	std::string line;
	while (std::getline(file, line))
	{
		std::istringstream ss(line);
		std::string token;
		std::vector<TileVisualType> row;

		while (ss >> token)
		{
			if (token == "D")
				row.push_back(TileVisualType::Dug_Spot);
			else if (token == "U")
				row.push_back(TileVisualType::Undug);
			else
				std::cerr << "[LevelLoader] Unknown tile type: " << token << std::endl;
		}

		m_InitialLayout.push_back(row);
	}

	std::cout << "[LevelLoader] Loaded level from file: " << fullPath << std::endl;
}

bool dae::LevelLoader::LoadFromBinary(const std::string& filename, LevelData& out)
{
	std::ifstream in{ ResourceManager::GetInstance().GetFullPath(filename), std::ios::binary };
	if (!in) return false;

	// 1) magic
	char magic[4];
	in.read(magic, 4);
	if (std::memcmp(magic, "LVL1", 4) != 0) return false;

	// 2) dims
	in.read(reinterpret_cast<char*>(&out.width), 2);
	in.read(reinterpret_cast<char*>(&out.height), 2);

	// 3) tiles
	out.tiles.resize(out.width * out.height);
	in.read(reinterpret_cast<char*>(out.tiles.data()), out.tiles.size());

	// 4) money bags
	uint16_t mcount;
	in.read(reinterpret_cast<char*>(&mcount), 2);
	out.moneyPos.resize(mcount);

	for (auto& p : out.moneyPos) 
	{
		uint8_t x, y;
		in.read(reinterpret_cast<char*>(&x), 1);
		in.read(reinterpret_cast<char*>(&y), 1);
		p = { int(x), int(y) };
	}

	// 5) gems
	uint16_t gcount;
	in.read(reinterpret_cast<char*>(&gcount), 2);
	out.gemPos.resize(gcount);

	for (auto& p : out.gemPos) 
	{
		uint8_t x, y;
		in.read(reinterpret_cast<char*>(&x), 1);
		in.read(reinterpret_cast<char*>(&y), 1);
		p = { int(x), int(y) };
	}

	return true;
}

void dae::LevelLoader::LoadLevel(Scene& scene, std::vector<std::vector<std::shared_ptr<dae::GameObject>>>& outTileGrid)
{
	LoadLevelFromFile("Level1.txt");

	const int LevelPixelWidth = TileWidth * NumCols;
	//const int LevelPixelHeight = TileHeight * NumRows;

	const int HorizontalOffset = (ScreenWidth - LevelPixelWidth) / 2; // ~5
	const int VerticalOffset = TopMargin; // Leave space for UI

	outTileGrid.resize(m_LevelHeight, std::vector<std::shared_ptr<GameObject>>(m_LevelWidth));

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

			tile->AddComponent<GridOutlineComponent>(TileWidth, TileHeight);

			scene.Add(tile);

			outTileGrid[y][x] = tile;
		}
	}
}

void dae::LevelLoader::LoadLevelBinary(const std::string& filename, Scene& scene,
	std::vector<std::vector<std::shared_ptr<GameObject>>>& outTileGrid, LevelManagerComponent* lvlMgr,
	TileManagerComponent* tileMgr)
{
	// 1) Read the .lvl file into a LevelData struct
	LevelData data;
	if (!LoadFromBinary(filename, data)) {
		throw std::runtime_error("Failed to load " + filename);
	}

	// 2) Resize outTileGrid
	outTileGrid.clear();
	outTileGrid.resize(data.height,
		std::vector<std::shared_ptr<GameObject>>(data.width));

	const int LevelPixelWidth = TileWidth * NumCols;
	//const int LevelPixelHeight = TileHeight * NumRows;

	const int HorizontalOffset = (ScreenWidth - LevelPixelWidth) / 2; // ~5
	const int VerticalOffset = TopMargin; // Leave space for UI

	for (int y = 0; y < data.height; ++y) {
		for (int x = 0; x < data.width; ++x) {
			uint8_t t = data.tiles[y * data.width + x];
			TileVisualType type = t
				? TileVisualType::Dug_Spot
				: TileVisualType::Undug;

			// create & position
			auto tileGO = std::make_shared<dae::GameObject>();
			float posX = float(x * GridSettings::TileWidth + HorizontalOffset);
			float posY = float(y * GridSettings::TileHeight + VerticalOffset);
			tileGO->GetTransform()->SetPosition(posX, posY, 0.0f);

			// render
			std::string tex = GetTexturePathForType(type);
			auto render = tileGO->AddComponent<dae::RenderComponent>(tex);
			render->SetSize(GridSettings::TileWidth, GridSettings::TileHeight);

			// logic
			auto logic = tileGO->AddComponent<TileComponent>(type);
			logic->SetRenderComponent(render.get());

			// debug grid
			tileGO->AddComponent<GridOutlineComponent>(
				GridSettings::TileWidth,
				GridSettings::TileHeight
			);

			scene.Add(tileGO);
			outTileGrid[y][x] = tileGO;
		}
	}

	// 4) Spawn MoneyBags
	for (auto& p : data.moneyPos) {
		dae::CreateMoneyBag(scene, *this, p.x, p.y,
			/*tileMgr*/tileMgr,
			/*lvlMgr*/lvlMgr);
	}

	// 5) Spawn Gems
	for (auto& p : data.gemPos) {
		dae::CreateGem(scene, *this, p.x, p.y);
	}
}

glm::vec3 dae::LevelLoader::GetWorldCenterForTile(int tileX, int tileY) const
{
	float x = static_cast<float>(tileX * TileWidth + HorOffset);
	float y = static_cast<float>(tileY * TileHeight + TileHeight / 2 + TopMargin);
	return { x, y, 0.0f };
}
