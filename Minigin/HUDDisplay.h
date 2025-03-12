#pragma once
#include "Observer.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

namespace dae
{
    class HUDDisplay : public Observer, public std::enable_shared_from_this<HUDDisplay>
    {
    public:
        HUDDisplay() = default;
        static std::shared_ptr<HUDDisplay> GetInstance();
        void Initialize(TextComponent* healthTextComponent, TextComponent* scoreTextComponent, HealthComponent* healthComponent, ScoreComponent* m_scoreComponent);
        void Notify(EventId event, GameObject* gameObject) override;

    private:
        TextComponent* m_pHealthTextComponent;
        TextComponent* m_ScoreTextComponent;
        HealthComponent* m_pHealthComponent;
        ScoreComponent* m_pScoreComponent;
    };
}