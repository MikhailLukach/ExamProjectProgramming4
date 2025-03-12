#include "HUDDisplay.h"
#include <string>





std::shared_ptr<dae::HUDDisplay> dae::HUDDisplay::GetInstance()
{
    static std::shared_ptr<HUDDisplay> instance = std::make_shared<HUDDisplay>();
    return instance;
}

void dae::HUDDisplay::Initialize(std::shared_ptr<TextComponent> textComponent, std::shared_ptr<HealthComponent> healthComponent)
{
    m_TextComponent = textComponent;
    m_pHealthComponent = healthComponent;
}

void dae::HUDDisplay::Notify(EventId event, GameObject* gameObject)
{
    (void) gameObject;
    int currentLives = m_pHealthComponent->GetLives();
    if (event == EventId::PLAYER_DAMAGED)
    {
        if (m_TextComponent)
        {
            std::cout << "[DEBUG] HUD received PLAYER_DAMAGED event! Updating text...\n";
            m_TextComponent->SetText("# lives: " + std::to_string(currentLives));
        }
        else
        {
            std::cout << "[ERROR] HUDDisplay has no TextComponent!\n";
        }
    }
    else if (event == EventId::PLAYER_DIED)
    {
        std::cout << "[DEBUG] HUD received PLAYER_DIED event! Updating text...\n";
        m_TextComponent->SetText("# lives: " + std::to_string(currentLives));

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "PlayerDied", "The player has died!", nullptr);
    }
}
