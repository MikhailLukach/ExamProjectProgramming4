#pragma once
#include <glm.hpp>

namespace dae
{
	class GameObject;
	class TileManagerComponent;

	namespace AIUtils
	{
		// Tries to move the agent in the specified tile direction if that tile is walkable (dug).
		void MoveTowards(GameObject* agent, const glm::ivec2& direction, float speed, TileManagerComponent* tileManager);
	}
}

