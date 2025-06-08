#include "FallingState.h"
#include "MoneyBagComponent.h"
#include <iostream>
#include "BreakingState.h"
#include "IdleState.h"
#include "TileManagerComponent.h"
#include "NobbinControllerComponent.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "TileTrackerComponent.h"
#include "GettingCrushedState.h"

void dae::FallingState::OnEnter(MoneyBagComponent& bag)
{
	std::cout << "[MoneyBag] Entering FallingState\n";
    bag.AddFallDistance(1); 
    std::cout << "[FallingState] Entered. Initial tile fallen.\n";

    auto tracker = bag.GetOwner()->GetComponent<TileTrackerComponent>();
    if (!tracker) return;

    auto tileManager = bag.GetTileManager();
    if (!tileManager) return;

    glm::ivec2 current = tracker->GetTileCoords();
    glm::ivec2 next = current;

    int maxFall = 0;

    while (true)
    {
        next += glm::ivec2{ 0, 1 };
        auto tile = tileManager->GetTileAt(next.x, next.y);
        if (!tile) break;

        auto comp = tile->GetComponent<TileComponent>();
        if (!comp || comp->GetType() != TileVisualType::Dug_Spot)
            break;

        ++maxFall;
    }

    bag.AddFallDistance(maxFall); 
    glm::ivec2 targetTile = current + glm::ivec2{ 0, maxFall };

    auto finalTile = tileManager->GetTileAt(targetTile.x, targetTile.y);
    if (!finalTile) return;

    float secondsPerTile = 0.2f; 
    float totalDuration = secondsPerTile * (maxFall + 1); 

    glm::vec3 worldPos = finalTile->GetTransform()->GetWorldPosition() + glm::vec3{ 6.f, 20.f, 0.f };
    bag.StartMoveTo(worldPos, totalDuration);

    std::cout << "[FallingState] Falling down " << maxFall + 1 << " tiles.\n";
}

std::unique_ptr<dae::MoneyBagState> dae::FallingState::Update(MoneyBagComponent& bag, float deltaTime)
{
    (void)deltaTime;
    if (!bag.GetIsMoving())
    {
        const int fallDistance = bag.GetFallDistance();
        std::cout << "[FallingState] Landed after falling " << fallDistance << " tiles.\n";

        if (fallDistance > 2)
        {
            std::cout << "[FallingState] Switching to BreakingState.\n";
            return std::make_unique<BreakingState>();
        }

        std::cout << "[FallingState] Switching to IdleState.\n";
        return std::make_unique<IdleState>();
    }

    return nullptr; 
}

void dae::FallingState::OnExit(MoneyBagComponent& bag)
{
    (void)bag;
    std::cout << "[MoneyBag] Exited FallingState\n";
}
