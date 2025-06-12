#include "ChasingState.h"
#include "NobbinControllerComponent.h"
#include "TileTrackerComponent.h"
#include "TileManagerComponent.h"
#include "LevelManagerComponent.h"
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

    // get our own tile and subsystems
    auto* owner = controller.GetOwner();
    auto tracker = owner ? owner->GetComponent<TileTrackerComponent>() : nullptr;
    auto* tileMgr = controller.GetTileManager();
    auto* lvlMgr = controller.GetLevelManager();
    if (!tracker || !tileMgr || !lvlMgr) return;

    const glm::ivec2 myTile = tracker->GetTileCoords();

    auto players = lvlMgr->GetAllPlayers();
    auto [targetPlayer, targetTile] =
        NobbinControllerComponent::GetNearestPlayer(players, myTile);
    if (!targetPlayer) return;

    auto path = controller.FindPath(myTile, targetTile);
    if (path.size() > 1)
    {
        glm::ivec2 dir = path[1] - myTile;
        controller.TryMoveInDirection(dir);
        return;
    }

    std::vector<glm::ivec2> dirs = { {1,0},{-1,0},{0,1},{0,-1} };
    std::sort(dirs.begin(), dirs.end(), [&](auto a, auto b) {
        return controller.GetTileCost(myTile + a)
            < controller.GetTileCost(myTile + b);
        });

    glm::ivec2 back; bool moved = false;
    for (auto d : dirs)
    {
        glm::ivec2 next = myTile + d;
        if (next == controller.GetPreviousTile()) { back = d; continue; }
        if (controller.IsDugTile(next))
        {
            controller.TryMoveInDirection(d);
            moved = true;
            break;
        }
    }
    if (!moved && back != glm::ivec2{ 0,0 })
    {
        std::cout << "[ChasingState] No forward move, backtracking.\n";
        controller.TryMoveInDirection(back);
    }
}

void dae::ChasingState::OnExit(NobbinControllerComponent& controller)
{
	(void)controller;
}
