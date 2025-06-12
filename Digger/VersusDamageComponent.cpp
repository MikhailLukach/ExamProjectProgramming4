#include "VersusDamageComponent.h"
#include "LevelManagerComponent.h"
#include "MoneyBagComponent.h"
#include "GameObject.h"
#include "LivesComponent.h"
#include "Scene.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "RenderComponent.h"
#include "Transform.h"
#include <glm.hpp>
#include <SDL.h>
#include <cmath>

dae::VersusDamageComponent::VersusDamageComponent(LevelManagerComponent* levelManager, const glm::vec3& spawnPos, float hitCooldownSeconds)
    : m_pLevelManager(levelManager)
    , m_CooldownTimer(0.f)
    , m_HitCooldownSecs(hitCooldownSeconds)
    , m_SpawnPos(spawnPos)
{
}

void dae::VersusDamageComponent::Update(float deltaTime)
{
    auto* owner = GetOwner();
    if (!owner) return;

    if (m_CooldownTimer > 0.f)
    {
        m_CooldownTimer -= deltaTime;

        if (m_NeedsRespawn && m_CooldownTimer <= 0.f)
        {
            owner->GetTransform()->SetPosition(m_SpawnPos);
            if (auto r = owner->GetComponent<RenderComponent>())
                r->SetVisible(true);

            dae::InputManager::GetInstance().EnableInputFor(owner);

            m_NeedsRespawn = false;
        }
    }

    if (m_CooldownTimer > 0.f || m_NeedsRespawn)
        return;

    auto  myPos = owner->GetTransform()->GetWorldPosition();
    const float radius = 16.f;

    for (auto* player : m_pLevelManager->GetAllPlayers())
    {
        if (!player || player == owner)
            continue;

        auto pos = player->GetTransform()->GetWorldPosition();
        if (std::abs(pos.x - myPos.x) < radius &&
            std::abs(pos.y - myPos.y) < radius)
        {
            if (auto lives = player->GetComponent<LivesComponent>())
            {
                lives->LoseLife();

                if (auto r = owner->GetComponent<RenderComponent>())
                    r->SetVisible(false);

                dae::InputManager::GetInstance().DisableInputFor(owner);

                m_CooldownTimer = m_HitCooldownSecs;
                m_NeedsRespawn = true;
            }
            break;
        }
    }

    auto scene = dae::SceneManager::GetInstance().GetCurrentScene();
    if (scene)
    {
        for (auto& obj : scene->GetObjects())
        {
            if (!obj || obj.get() == owner)
                continue;

            auto bagComp = obj->GetComponent<MoneyBagComponent>();
            if (!bagComp || !bagComp->IsFalling())
                continue;

            auto bagPos = obj->GetTransform()->GetWorldPosition();
            if (std::abs(bagPos.x - myPos.x) < radius &&
                std::abs(bagPos.y - myPos.y) < radius)
            {
                // crushed by bag trigger Nobbin respawn
                if (auto r = owner->GetComponent<RenderComponent>())
                    r->SetVisible(false);
                m_CooldownTimer = m_HitCooldownSecs;
                m_NeedsRespawn = true;
                return;  // done
            }
        }
    }

}

void dae::VersusDamageComponent::SetNeedsToRespawn(bool needsToRespawn)
{
    if (needsToRespawn && !m_NeedsRespawn)
    {
        // first time we’re entering respawn
        m_NeedsRespawn = true;
        m_CooldownTimer = m_HitCooldownSecs;

        // hide immediately
        if (auto r = GetOwner()->GetComponent<RenderComponent>())
            r->SetVisible(false);
    }
    else if (!needsToRespawn)
    {
        // if you ever want to cancel
        m_NeedsRespawn = false;
        m_CooldownTimer = 0.f;
    }
}

