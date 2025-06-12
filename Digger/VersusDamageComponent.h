#pragma once
#include "Component.h"
#include <glm.hpp>
#include <cstdint>   

namespace dae
{
    class LevelManagerComponent;
    class VersusDamageComponent : public Component
    {
    public:
        VersusDamageComponent(LevelManagerComponent* levelManager, const glm::vec3& spawnPos, float hitCooldownSeconds = 5.f);

        void Update(float deltaTime) override;

        void SetNeedsToRespawn(bool needsToRespawn);

    private:
        LevelManagerComponent* m_pLevelManager;

        bool m_HasHitPlayer = false;
        bool m_NeedsRespawn = false;

        glm::vec3 m_SpawnPos;
        float m_HitCooldownSecs;
        float m_CooldownTimer;
    };
}

