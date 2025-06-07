#include "ChasingAndDiggingState.h"
#include "NobbinControllerComponent.h"
#include "SpriteAnimatorComponent.h"
#include "NobbinComponent.h"
#include "TileTrackerComponent.h"
#include "TileComponent.h"
#include "TileManagerComponent.h"
#include "MoneyBagComponent.h"
#include "GemComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"

#include <glm.hpp>
#include <algorithm>
#include <iostream>

void dae::ChasingAndDiggingState::OnEnter(dae::NobbinControllerComponent& controller)
{
    std::cout << "[ChasingAndDiggingState] Nobbin is now a Hobbin!\n";

    auto* owner = controller.GetOwner();
    if (!owner)
    {
        std::cerr << "[ChasingAndDiggingState] ERROR: Controller's GameObject is null!\n";
        return;
    }

    auto render = controller.GetOwner()->GetComponent<dae::RenderComponent>();
    auto animator = controller.GetOwner()->GetComponent<dae::SpriteAnimatorComponent>();

    if (render)
    {
        render->SetTexture("HobbinSpriteSheetVer2.png"); // Swap the sprite sheet
        render->SetSize(32, 32);                     // Ensure it matches Hobbin frames
        render->SetRenderOffset({ 0.f, -16.f });       // Optional: adjust if needed
    }

    if (animator)
    {
        // Reconfigure if Hobbin sheet has different frame size or timing
        animator->Configure(render.get(), 16, 16, 0.12f); // Adjust values as needed
        animator->PlayAnimation(6, 3); // Start with default anim
    }
}

void dae::ChasingAndDiggingState::Update(dae::NobbinControllerComponent& controller, float deltaTime)
{
    (void)deltaTime;
    auto pOwner = controller.GetOwner();
    if (pOwner)
    {
        auto transform = pOwner->GetTransform();
        glm::vec3 pos = transform->GetWorldPosition();

        SDL_Rect nobbinRect{
            static_cast<int>(pos.x),
            static_cast<int>(pos.y),
            32, 32
        };

        auto* scene = dae::SceneManager::GetInstance().GetCurrentScene();
        if (scene)
        {
            for (auto& obj : scene->GetObjects())
            {
                if (obj.get() == pOwner) continue;

                auto gem = obj->GetComponent<dae::GemComponent>();
                if (gem)
                {
                    auto objTransform = obj->GetTransform();
                    glm::vec3 objPos = objTransform->GetWorldPosition();

                    SDL_Rect objRect{
                        static_cast<int>(objPos.x),
                        static_cast<int>(objPos.y),
                        32, 32
                    };

                    if (SDL_HasIntersection(&nobbinRect, &objRect))
                    {
                        std::cout << "[ChasingAndDiggingState] Destroyed Gem!\n";
                        obj->MarkForDeletion();
                    }

                    continue; // don't try to also treat gem as moneybag
                }

                // Check for MoneyBag next
                auto moneyBag = obj->GetComponent<dae::MoneyBagComponent>();
                if (moneyBag)
                {
                    // Check state: must not be FallingState
                    if (!moneyBag->IsFalling()) // You'll need this helper
                    {
                        auto objTransform = obj->GetTransform();
                        glm::vec3 objPos = objTransform->GetWorldPosition();

                        SDL_Rect objRect{
                            static_cast<int>(objPos.x),
                            static_cast<int>(objPos.y),
                            32, 32
                        };

                        if (SDL_HasIntersection(&nobbinRect, &objRect))
                        {
                            std::cout << "[ChasingAndDiggingState] Destroyed MoneyBag!\n";
                            obj->MarkForDeletion();
                        }
                    }
                }
            }
        }
    }

    auto tracker = controller.GetTracker();
    auto player = controller.GetPlayer();
    auto tileManager = controller.GetTileManager();
    auto mover = controller.GetOwner()->GetComponent<dae::NobbinComponent>();

    if (!tracker || !player || !tileManager || !mover || mover->NobbinIsMoving())
        return;

    const auto myTile = tracker->GetTileCoords();
    const auto playerTracker = player->GetComponent<dae::TileTrackerComponent>();
    if (!playerTracker) return;

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

        // Always allow movement, but dig if needed
        auto tileObj = tileManager->GetTileAt(nextTile.x, nextTile.y);
        if (tileObj)
        {
            auto tileComp = tileObj->GetComponent<dae::TileComponent>();
            if (tileComp && tileComp->GetType() != TileVisualType::Dug_Spot)
            {
                tileComp->SetType(TileVisualType::Dug_Spot); // Dig through it!
            }
        }

        controller.TryMoveInDirection(dir);
        m_LastMoveDir = dir;
        return;
    }
}

void dae::ChasingAndDiggingState::OnExit(dae::NobbinControllerComponent& controller)
{
    (void)controller;
    std::cout << "[ChasingAndDiggingState] Nobbin is no longer digging.\n";
}