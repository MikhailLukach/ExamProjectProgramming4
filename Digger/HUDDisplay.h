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
        HUDDisplay(TextComponent* scoreText, ScoreComponent* score);

        void Notify(EventId event, GameObject* gameObject) override;

    private:
        TextComponent* m_pScoreTextComponent;
        ScoreComponent* m_pScoreComponent;
    };
}
