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
#include <CollisionHelper.h>
#include <SoundServiceLocator.h>

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

    for (auto* player : m_pLevelManager->GetAllPlayers())
    {
        if (!player || player == owner)
            continue;

        if (!player->GetComponent<RenderComponent>() || !owner->GetComponent<RenderComponent>())
            continue;

        if (CheckRenderComponentCollision(owner, player))
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
    if (!scene) return;

    for (auto& obj : scene->GetObjects())
    {
        if (!obj || obj.get() == owner)
            continue;

        auto bagComp = obj->GetComponent<MoneyBagComponent>();
        if (!bagComp || !bagComp->IsFalling())
            continue;

        if (!obj->GetComponent<RenderComponent>() || !owner->GetComponent<RenderComponent>())
            continue;

        if (CheckRenderComponentCollision(owner, obj.get()))
        {
            // Crushed by falling bag, trigger Nobbin respawn
            if (auto r = owner->GetComponent<RenderComponent>())
                r->SetVisible(false);

            dae::SoundServiceLocator::Get().PlaySound(dae::ResourceManager::GetInstance().GetFullPath("AppleCrushes.wav"));

            m_CooldownTimer = m_HitCooldownSecs;
            m_NeedsRespawn = true;
            return;
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

        dae::InputManager::GetInstance().DisableInputFor(GetOwner());
    }
    else if (!needsToRespawn)
    {
        // if you ever want to cancel
        m_NeedsRespawn = false;
        m_CooldownTimer = 0.f;
    }
}

