#pragma once
#include "Observer.h"
#include "TextComponent.h"
#include "HealthComponent.h"

namespace dae
{
    class HUDDisplay : public Observer, public std::enable_shared_from_this<HUDDisplay>
    {
    public:
        HUDDisplay() = default;
        static std::shared_ptr<HUDDisplay> GetInstance();
        void Initialize(std::shared_ptr<TextComponent> textComponent, std::shared_ptr<HealthComponent> healthComponent);
        void Notify(EventId event, GameObject* gameObject) override;

    private:
        std::shared_ptr<TextComponent> m_TextComponent;
        std::shared_ptr<HealthComponent> m_pHealthComponent;
    };
}