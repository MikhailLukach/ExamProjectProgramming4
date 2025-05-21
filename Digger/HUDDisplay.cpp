#include "HUDDisplay.h"
//#include "steam_api.h"
#include <string>

dae::HUDDisplay::HUDDisplay(TextComponent* scoreText, ScoreComponent* score)
    : m_pScoreTextComponent(scoreText), m_pScoreComponent(score)
{
}

void dae::HUDDisplay::Notify(EventId event, GameObject* gameObject)
{
    (void) gameObject;
    int currentScore = m_pScoreComponent->GetScore();
    if (event == EventId::PLAYER_ADDSCORE)
    {
        std::cout << "[DEBUG] HUD received PLAYER_ADDSCORE event! Updating text...\n";
        m_pScoreTextComponent->SetText("# score: " + std::to_string(currentScore));
    }
    else if(event == EventId::PLAYER_REACHED_500_POINTS)
    {
        //check whether the player already has the acheivement
        //remove yourself as an observer, since the achievement has been done
        std::cout << "[DEBUG] HUD received PLAYER_REACHED_500_POINTS event! Updating achievements...\n";
        //SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
        //SteamUserStats()->StoreStats();
    }
}
