#pragma once
#include <memory>
#include "GameObject.h"

namespace dae
{
	class TileManagerComponent;
	class LevelManagerComponent;
	class Scene;
	class LevelLoader;

	std::shared_ptr<GameObject> CreateMoneyBag(Scene& scene, LevelLoader& loader, int tileX, int tileY,
		TileManagerComponent* tileManager, LevelManagerComponent* levelManager);

	std::shared_ptr<GameObject> CreateGem(Scene& scene, LevelLoader& loader, int tileX, int tileY);
}

