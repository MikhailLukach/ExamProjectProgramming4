#include "AIUtils.h"
#include "GameObject.h"
#include "Transform.h"
#include "TileManagerComponent.h"
#include "TileTrackerComponent.h"
#include "TileComponent.h"
#include "TileTypes.h"
#include "GridSettings.h"
#include <glm.hpp>
#include <cmath>
#include <iostream>

void dae::AIUtils::MoveTowards(GameObject* agent, const glm::ivec2& direction, float speed, TileManagerComponent* tileManager)
{
	if (!agent || !tileManager) return;

	auto tracker = agent->GetComponent<TileTrackerComponent>();
	if (!tracker) return;

	auto currentTile = tracker->GetTileCoords();
	auto nextTile = currentTile + direction;

	std::cout << "[Nobbin] Trying to move to: (" << nextTile.x << ", " << nextTile.y << ")\n";

	auto tileObj = tileManager->GetTileAt(nextTile.x, nextTile.y);
	if (!tileObj) return;

	auto tileComp = tileObj->GetComponent<TileComponent>();
	if (!tileComp || tileComp->GetType() != TileVisualType::Dug_Spot) 
	{
		std::cout << "[Nobbin] Tile is not dug — cannot move.\n";
		return; 
	}

	std::cout << "[Nobbin] Moving in direction: (" << direction.x << ", " << direction.y << ")\n";
	// Valid tile: apply movement
	glm::vec3 worldDir = glm::normalize(glm::vec3(direction.x, direction.y, 0.0f));
	auto currentPos = agent->GetTransform()->GetWorldPosition();
	agent->GetTransform()->SetPosition(currentPos + worldDir * speed);
}
