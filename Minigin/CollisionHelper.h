#pragma once
#include <SDL.h>
#include <memory>
#include "GameObject.h"
#include "RenderComponent.h"

namespace dae
{
    inline SDL_Rect GetWorldRenderRect(GameObject* obj)
    {
        if (!obj) return { 0, 0, 0, 0 };

        auto render = obj->GetComponent<RenderComponent>();
        if (!render) return { 0, 0, 0, 0 };

        const auto pos = obj->GetTransform()->GetWorldPosition();

        return SDL_Rect{
            static_cast<int>(std::round(pos.x + render->GetRenderOffset().x)),
            static_cast<int>(std::round(pos.y + render->GetRenderOffset().y)),
            static_cast<int>(render->GetWidth()),
            static_cast<int>(render->GetHeight())
        };
    }

    inline bool CheckAABBCollision(const SDL_Rect& a, const SDL_Rect& b)
    {
        return a.x < b.x + b.w &&
            a.x + a.w > b.x &&
            a.y < b.y + b.h &&
            a.y + a.h > b.y;
    }

    inline bool CheckRenderComponentCollision(GameObject* a, GameObject* b)
    {
        return CheckAABBCollision(GetWorldRenderRect(a), GetWorldRenderRect(b));
    }
}