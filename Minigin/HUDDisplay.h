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
        void Initialize(std::shared_ptr<TextComponent> healthTextComponent, std::shared_ptr<TextComponent> scoreTextComponent
            , std::shared_ptr<HealthComponent> healthComponent, std::shared_ptr<ScoreComponent> m_scoreComponent);
        void Notify(EventId event, GameObject* gameObject) override;

    private:
        std::shared_ptr<TextComponent> m_pHealthTextComponent;
        std::shared_ptr<TextComponent> m_ScoreTextComponent;
        std::shared_ptr<HealthComponent> m_pHealthComponent;
        std::shared_ptr<ScoreComponent> m_pScoreComponent;
    };
}