#include "FireBallComponent.h"
#include "TileTrackerComponent.h"
#include "TileManagerComponent.h"
#include "SceneManager.h"
#include "NobbinControllerComponent.h"
#include "GameObject.h"
#include "GridSettings.h"
#include "Scene.h"
#include "TileComponent.h"

dae::FireBallComponent::FireBallComponent(const glm::vec2& direction, float speed, float lifeSec, TileManagerComponent* tileManager)
	: m_Dir(direction), m_Speed(speed), m_RemainingLife(lifeSec), m_pTileManager(tileManager)
{
}

void dae::FireBallComponent::Render() const
{
    SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();
    if (!renderer) return;

    auto pos = GetOwner()->GetTransform()->GetWorldPosition();
    SDL_Rect rect;
    rect.x = static_cast<int>(pos.x) + 12;
    rect.y = static_cast<int>(pos.y) + 12;
    rect.w = 16;
    rect.h = 16;

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
    SDL_RenderDrawRect(renderer, &rect);
}

void dae::FireBallComponent::Update(float deltaTime)
{
    auto t = GetOwner()->GetTransform();
    glm::vec3 pos = t->GetWorldPosition();
    pos += glm::vec3(m_Dir * m_Speed * deltaTime, 0);
    t->SetWorldPosition(pos);

    if (m_pTileManager)
    {
        // Convert world pos to tile indices via TileTracker logic
        // We'll use a temporary TileTrackerComponent to get coords:
        TileTrackerComponent tracker(
            GridSettings::TileWidth,
            GridSettings::TileHeight,
            GridSettings::OffsetX,
            GridSettings::OffsetY
        );
        tracker.SetOwner(GetOwner());
        tracker.Update(0);
        auto tileCoords = tracker.GetTileCoords();

        auto tileObj = m_pTileManager->GetTileAt(tileCoords.x, tileCoords.y);
        if (tileObj)
        {
            auto tileComp = tileObj->GetComponent<TileComponent>();
            if (tileComp && tileComp->GetType() == TileVisualType::Undug)
            {
                GetOwner()->MarkForDeletion();
                return;
            }
        }
    }

    auto* scene = SceneManager::GetInstance().GetCurrentScene();
    if (scene)
    {
        SDL_Rect fbR{ int(pos.x) + 12, int(pos.y) + 12, 16, 16 };
        for (auto& obj : scene->GetObjects())
        {
            if (auto nobCtrl = obj->GetComponent<NobbinControllerComponent>())
            {
                auto p = obj->GetTransform()->GetWorldPosition();
                SDL_Rect nbR{ int(p.x), int(p.y), 32, 32 };
                if (SDL_HasIntersection(&fbR, &nbR))
                {
                    std::cout << "[FireBallComponent] intersects with Nobbin" << std::endl;
                    obj->MarkForDeletion();         
                    GetOwner()->MarkForDeletion(); 
                    return;
                }
            }
        }
    }

    m_RemainingLife -= deltaTime;
    if (m_RemainingLife <= 0.f)
        GetOwner()->MarkForDeletion();
}
