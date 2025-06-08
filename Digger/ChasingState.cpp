#include "ChasingState.h"
#include "NobbinControllerComponent.h"
#include "TileTrackerComponent.h"
#include "TileManagerComponent.h"
#include "ChasingAndDiggingState.h"
#include "GameObject.h"

#include <glm.hpp>
#include <algorithm>
#include <iostream>

void dae::ChasingState::OnEnter(NobbinControllerComponent& controller)
{
	(void)controller;
}

void dae::ChasingState::Update(NobbinControllerComponent& controller, float deltaTime)
{
	m_AngerTimer += deltaTime;
	//std::cout << "[ChasingTime] AngerTime = " << m_AngerTimer << std::endl;
	if (m_AngerTimer > m_AngerThreshold)
	{
		std::cout << "[ChasingState] Getting angry, switching to ChasingAndDiggingState.\n";
		controller.ChangeState(std::make_unique<ChasingAndDiggingState>());
		return;
	}

	auto pOwner = controller.GetOwner();
	if (!pOwner) return;

	auto pTracker = pOwner->GetComponent<TileTrackerComponent>();
	auto pPlayer = controller.GetPlayer();
	auto pTileManager = controller.GetTileManager();

	if (!pTracker || !pPlayer || !pTileManager) return;

	auto playerTracker = pPlayer->GetComponent<TileTrackerComponent>();
	if (!playerTracker) return;

	const auto myTile = pTracker->GetTileCoords();
	const auto playerTile = playerTracker->GetTileCoords();

	std::vector<glm::ivec2> directions = {
		{1, 0}, {-1, 0}, {0, 1}, {0, -1}
	};

	std::sort(directions.begin(), directions.end(), [&](const glm::ivec2& a, const glm::ivec2& b) {
		glm::ivec2 posA = myTile + a;
		glm::ivec2 posB = myTile + b;
		int distA = glm::abs(posA.x - playerTile.x) + glm::abs(posA.y - playerTile.y);
		int distB = glm::abs(posB.x - playerTile.x) + glm::abs(posB.y - playerTile.y);
		return distA < distB;
		});

	for (const auto& dir : directions)
	{
		glm::ivec2 nextTile = myTile + dir;

		if (nextTile == controller.GetPreviousTile()) continue;

		// IsDugTile is a helper in controller, we can use it:
		if (controller.IsDugTile(nextTile))
		{
			controller.TryMoveInDirection(dir);
			return;
		}
	}
}

void dae::ChasingState::OnExit(NobbinControllerComponent& controller)
{
	(void)controller;
}
