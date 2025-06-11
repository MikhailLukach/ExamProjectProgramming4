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
    if (m_AngerTimer > m_AngerThreshold)
    {
        std::cout << "[ChasingState] Getting angry, switching to ChasingAndDiggingState.\n";
        controller.ChangeState(std::make_unique<ChasingAndDiggingState>());
        return;
    }

    auto pOwner = controller.GetOwner();
    auto pTracker = pOwner ? pOwner->GetComponent<TileTrackerComponent>() : nullptr;
    auto pPlayer = controller.GetPlayer();
    auto pTileManager = controller.GetTileManager();
    if (!pTracker || !pPlayer || !pTileManager) return;

    const glm::ivec2 myTile = pTracker->GetTileCoords();
    const glm::ivec2 playerTile = pPlayer->GetComponent<TileTrackerComponent>()->GetTileCoords();

    // 1) Try A* path
    auto path = controller.FindPath(myTile, playerTile);
    if (path.size() > 1)
    {
        glm::ivec2 nextTile = path[1];
        glm::ivec2 dir = nextTile - myTile;
        controller.TryMoveInDirection(dir);
        return;
    }

    // 2) Fallback: digging or backtracking
    std::vector<glm::ivec2> directions = { {1,0},{-1,0},{0,1},{0,-1} };
    // sort by our tile cost heuristic, so we still favor dug tunnels over dirt
    std::sort(directions.begin(), directions.end(), [&](auto a, auto b) {
        return controller.GetTileCost(myTile + a)
            < controller.GetTileCost(myTile + b);
        });

    glm::ivec2 backtrackDir{};
    bool      moved = false;

    for (auto dir : directions)
    {
        glm::ivec2 nextTile = myTile + dir;
        if (nextTile == controller.GetPreviousTile())
        {
            backtrackDir = dir;
            continue;
        }

        if (controller.IsDugTile(nextTile))
        {
            controller.TryMoveInDirection(dir);
            moved = true;
            break;
        }
    }

    if (!moved && backtrackDir != glm::ivec2{ 0,0 })
    {
        std::cout << "[ChasingState] No forward move possible, backtracking.\n";
        controller.TryMoveInDirection(backtrackDir);
    }
}

void dae::ChasingState::OnExit(NobbinControllerComponent& controller)
{
	(void)controller;
}
