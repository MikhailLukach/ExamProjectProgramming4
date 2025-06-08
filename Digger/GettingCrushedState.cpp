#include "GettingCrushedState.h"
#include "NobbinControllerComponent.h"
#include "LevelManagerComponent.h"
#include "TileManagerComponent.h"
#include "TileTrackerComponent.h"
#include "LevelLoader.h"
#include "Transform.h"
#include "GameObject.h"
#include "MoneyBagComponent.h"
#include "TileComponent.h"

void dae::GettingCrushedState::OnEnter(NobbinControllerComponent& controller)
{
    std::cout << "[Nobbin] Entered GettingCrushedState.\n";

    auto tracker = controller.GetTracker();
    auto levelMgr = controller.GetLevelManager();

    if (!tracker || !levelMgr) return;

    glm::ivec2 tile = tracker->GetTileCoords();
    m_pCrushingBag = levelMgr->GetMoneyBagAt(tile.x, tile.y);
}

void dae::GettingCrushedState::Update(NobbinControllerComponent& controller, float deltaTime)
{
    auto nobbinObj = controller.GetOwner();
    if (!nobbinObj) return;

    auto transform = nobbinObj->GetTransform();
    auto tracker = controller.GetTracker();
    auto tileManager = controller.GetTileManager();
    if (!transform || !tracker || !tileManager) return;

    // Move down
    glm::vec3 pos = transform->GetWorldPosition();
    pos.y += m_FallSpeed * deltaTime;
    transform->SetPosition(pos);
    m_AccumulatedFall += m_FallSpeed * deltaTime;

    // Check tile below
    glm::ivec2 tileBelow = tracker->GetTileCoords() + glm::ivec2{ 0, 1 };
    auto belowTile = tileManager->GetTileAt(tileBelow.x, tileBelow.y);

    if (!belowTile)
    {
        std::cout << "[Nobbin] Fell off map. Destroying.\n";
        nobbinObj->MarkForDeletion();
        return;
    }

    auto tileComp = belowTile->GetComponent<TileComponent>();
    if (!tileComp || tileComp->GetType() != TileVisualType::Dug_Spot)
    {
        std::cout << "[Nobbin] Crushed and landed. Destroying.\n";
        nobbinObj->MarkForDeletion();
    }
}

void dae::GettingCrushedState::OnExit(NobbinControllerComponent& controller)
{
    (void)controller;
    std::cout << "[Nobbin] Exiting GettingCrushedState.\n";
}

