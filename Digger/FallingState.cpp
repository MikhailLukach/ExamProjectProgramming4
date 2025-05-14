#include "FallingState.h"
#include "MoneyBagComponent.h"
#include <iostream>
#include "BreakingState.h"
#include "IdleState.h"
#include "TileManagerComponent.h"
#include "TileComponent.h"
#include "GameObject.h"
#include "TileTrackerComponent.h"

void dae::FallingState::OnEnter(MoneyBagComponent& bag)
{
	std::cout << "[MoneyBag] Entering FallingState\n";
	bag.ResetFallDistance();
}

std::unique_ptr<dae::MoneyBagState> dae::FallingState::Update(MoneyBagComponent& bag, float deltaTime)
{
    m_FallTimer += deltaTime;
    std::cout << "[MoneyBag] It's falling!\n";
    if (m_FallTimer >= 0.5f)
    {
        m_FallTimer = 0.0f;
        bag.AddFallDistance(1);
        std::cout << "[MoneyBag] Falling... Total tiles fallen: " << bag.GetFallDistance() << "\n";

        auto tracker = bag.GetOwner()->GetComponent<TileTrackerComponent>();
        if (!tracker) return nullptr;

        glm::ivec2 currentTile = tracker->GetTileCoords();
        glm::ivec2 belowTile = currentTile + glm::ivec2{ 0, 1 };

        auto tileManager = bag.GetTileManager(); 
        auto tileBelow = tileManager ? tileManager->GetTileAt(belowTile.x, belowTile.y) : nullptr;

        bool isDugBelow = false;

        if (tileBelow)
        {
            auto tileComp = tileBelow->GetComponent<TileComponent>();
            if (tileComp)
                isDugBelow = (tileComp->GetType() == TileVisualType::Dug_Spot);
        }

        if (isDugBelow)
        {
            auto worldPos = tileBelow->GetTransform()->GetWorldPosition() + glm::vec3{ 6.f, 20.f, 0.f };
            bag.StartMoveTo(worldPos); // Smoothly fall into next tile
            return nullptr; // keep falling
        }
        else
        {
            if (bag.GetFallDistance() >= 2)
                return std::make_unique<BreakingState>();

            return std::make_unique<IdleState>();
        }
    }

    return nullptr;
}

void dae::FallingState::OnExit(MoneyBagComponent& bag)
{
    (void)bag;
    std::cout << "[MoneyBag] Exited FallingState\n";
}
