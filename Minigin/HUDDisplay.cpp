#include "HUDDisplay.h"
#include <string>

std::shared_ptr<dae::HUDDisplay> dae::HUDDisplay::GetInstance()
{
    static std::shared_ptr<HUDDisplay> instance = std::make_shared<HUDDisplay>();
    return instance;
}

void dae::HUDDisplay::Initialize(TextComponent* healthTextComponent, TextComponent* scoreTextComponent, HealthComponent* healthComponent
    , ScoreComponent* m_scoreComponent)
{
    m_pHealthTextComponent = healthTextComponent;
    m_ScoreTextComponent = scoreTextComponent;
    m_pHealthComponent = healthComponent;
    m_pScoreComponent = m_scoreComponent;
}

void dae::HUDDisplay::Notify(EventId event, GameObject* gameObject)
{
    (void) gameObject;
    int currentLives = m_pHealthComponent->GetLives();
    int currentScore = m_pScoreComponent->GetScore();
    if (event == EventId::PLAYER_DAMAGED)
    {
        if (m_pHealthTextComponent)
        {
            std::cout << "[DEBUG] HUD received PLAYER_DAMAGED event! Updating text...\n";
            m_pHealthTextComponent->SetText("# lives: " + std::to_string(currentLives));
        }
        else
        {
            std::cout << "[ERROR] HUDDisplay has no TextComponent!\n";
        }
    }
    else if (event == EventId::PLAYER_DIED)
    {
        std::cout << "[DEBUG] HUD received PLAYER_DIED event! Updating text...\n";
        m_pHealthTextComponent->SetText("# lives: " + std::to_string(currentLives));

        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "PlayerDied", "The player has died!", nullptr);
    }
    else if (event == EventId::PLAYER_ADDSCORE)
    {
        std::cout << "[DEBUG] HUD received PLAYER_ADDSCORE event! Updating text...\n";
        m_ScoreTextComponent->SetText("# score: " + std::to_string(currentScore));
    }
}
