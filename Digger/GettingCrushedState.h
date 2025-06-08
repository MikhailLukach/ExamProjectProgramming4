#pragma once
#include "NobbinState.h"
#include "GameObject.h"
#include <glm.hpp>

namespace dae
{
    class GettingCrushedState final : public NobbinState
    {
    public:
        void OnEnter(NobbinControllerComponent& controller) override;
        void Update(NobbinControllerComponent& controller, float deltaTime) override;
        void OnExit(NobbinControllerComponent& controller) override;

        const char* GetID() const override { return "GettingCrushed"; }
    private:
        float m_FallSpeed = 700.f; // pixels per second
        std::shared_ptr<GameObject> m_pCrushingBag;

        float m_AccumulatedFall = 0.f;
    };
}

