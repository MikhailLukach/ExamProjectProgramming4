#pragma once
#include "Observer.h"
#include "Component.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

namespace dae
{
    class HUDDisplay : public Component, public Observer
    {
    public:
        HUDDisplay(TextComponent* healthText, TextComponent* scoreText, HealthComponent* health, ScoreComponent* score);

        void Notify(EventId event, GameObject* gameObject) override;

    private:
        TextComponent* m_pHealthTextComponent;
        TextComponent* m_pScoreTextComponent;
        HealthComponent* m_pHealthComponent;
        ScoreComponent* m_pScoreComponent;
    };
}
