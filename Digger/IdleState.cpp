#include "IdleState.h"
#include <iostream>
#include "FallingState.h"
#include "TileManagerComponent.h"
#include "GameObject.h"
#include "TileTrackerComponent.h"
#include "MoneyBagComponent.h"
#include "SpriteAnimatorComponent.h"
#include "TileComponent.h"

void dae::IdleState::OnEnter(MoneyBagComponent& bag)
{
    bag.ResetFallDistance();
    std::cout << "[Debug] Entered IdleState\n";

    auto animator = bag.GetOwner()->GetComponent<SpriteAnimatorComponent>();
    if (animator)
    {
        animator->Stop();
    }
}

std::unique_ptr<dae::MoneyBagState> dae::IdleState::Update(MoneyBagComponent& bag, float deltaTime)
{
    auto tracker = bag.GetOwner()->GetComponent<TileTrackerComponent>();
    if (!tracker) return nullptr;

    glm::ivec2 currentTile = tracker->GetTileCoords();
    glm::ivec2 belowTile = currentTile + glm::ivec2{ 0, 1 };

    auto tileManager = bag.GetTileManager();
    auto tileBelow = tileManager ? tileManager->GetTileAt(belowTile.x, belowTile.y) : nullptr;

    if (tileBelow)
    {
        auto tileComp = tileBelow->GetComponent<TileComponent>();
        if (tileComp && tileComp->GetType() == TileVisualType::Dug_Spot)
        {
            if (bag.WasRecentlyAbovePlayer())
            {
                if (m_FallDelayTimer < 0.f)
                {
                    m_FallDelayTimer = 2.f; // start the delay
                    auto animator = bag.GetOwner()->GetComponent<SpriteAnimatorComponent>();
                    if (animator)
                    {
                        animator->PlayAnimation(0, 3); // 3 frames, from frame 0
                    }
                }
                else
                {
                    m_FallDelayTimer -= deltaTime;
                    if (m_FallDelayTimer <= 0.f)
                    {
                        return std::make_unique<FallingState>();
                    }
                }
            }
            else
            {
                return std::make_unique<FallingState>(); // pushed into shaft, fall immediately
            }
        }
        else
        {
            m_FallDelayTimer = -1.f; // reset if it's not dug
            auto animator = bag.GetOwner()->GetComponent<SpriteAnimatorComponent>();
            if (animator)
            {
                animator->Stop();
                SDL_Rect idleRect{};
                idleRect.x = 16; // middle frame
                idleRect.y = 0;
                idleRect.w = 16;
                idleRect.h = 15;

                auto render = bag.GetOwner()->GetComponent<RenderComponent>();
                render->SetSourceRect(idleRect);
            }
        }
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
    glm::vec3 worldPos = tilePos + glm::vec3{ 6.f, 20.f, 0.f };
    bag.StartMoveTo(worldPos);

    std::cout << "[IdleState] MoneyBag pushed to tile: (" << target.x << ", " << target.y << ")\n";
}
