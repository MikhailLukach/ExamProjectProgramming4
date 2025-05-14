#include "IdleState.h"
#include <iostream>
#include "FallingState.h"
#include "TileManagerComponent.h"
#include "GameObject.h"
#include "TileTrackerComponent.h"
#include "MoneyBagComponent.h"

void dae::IdleState::OnEnter(MoneyBagComponent& bag)
{
    (void)bag;
    std::cout << "[Debug] Entered IdleState\n";
}

std::unique_ptr<dae::MoneyBagState> dae::IdleState::Update(MoneyBagComponent& bag, float deltaTime)
{
    //std::cout << "[Debug] IdleState is running\n";
    (void)bag;
    (void)deltaTime;
    if (false) //check for tile below
    {
        std::cout << "[MoneyBag] Tile below is dug. Transition to Falling.\n";
        return std::make_unique<FallingState>();
    }

    return nullptr;
}

void dae::IdleState::OnExit(MoneyBagComponent& bag)
{
    (void)bag;
    std::cout << "[Debug] Exiting IdleState\n";
}

void dae::IdleState::TryPushHorizontally(MoneyBagComponent& bag, int direction, TileManagerComponent* tileManager)
{
    if (direction != -1 && direction != 1) return;
    if (!tileManager || !bag.GetOwner()) return;

    auto tracker = bag.GetOwner()->GetComponent<TileTrackerComponent>();
    if (!tracker) return;

    glm::ivec2 current = tracker->GetTileCoords();
    glm::ivec2 target = current + glm::ivec2{ direction, 0 };

    auto tile = tileManager->GetTileAt(target.x, target.y);
    if (!tile)
    {
        std::cout << "[IdleState] Push blocked: No tile at (" << target.x << ", " << target.y << ")\n";
        return;
    }

    glm::vec3 tilePos = tile->GetTransform()->GetWorldPosition();
    glm::vec3 worldPos = tilePos + glm::vec3{ 4.f, 20.f, 0.f };
    bag.StartMoveTo(worldPos);

    std::cout << "[IdleState] MoneyBag pushed to tile: (" << target.x << ", " << target.y << ")\n";
}
