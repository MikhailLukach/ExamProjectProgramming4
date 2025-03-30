#include "HUDDisplay.h"
#include "steam_api.h"
#include <string>

dae::HUDDisplay::HUDDisplay(TextComponent* healthText, TextComponent* scoreText, HealthComponent* health, ScoreComponent* score)
    : m_pHealthTextComponent(healthText), m_pScoreTextComponent(scoreText),
    m_pHealthComponent(health), m_pScoreComponent(score)
{
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
        m_pScoreTextComponent->SetText("# score: " + std::to_string(currentScore));
    }
    else if(event == EventId::PLAYER_REACHED_500_POINTS)
    {
        //check whether the player already has the acheivement
        //remove yourself as an observer, since the achievement has been done
        std::cout << "[DEBUG] HUD received PLAYER_REACHED_500_POINTS event! Updating achievements...\n";
        SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
        SteamUserStats()->StoreStats();
    }
}
