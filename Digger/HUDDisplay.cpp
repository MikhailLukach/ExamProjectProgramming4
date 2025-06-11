#include "HUDDisplay.h"
#include <string>
#include <iomanip>
#include <sstream> 

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
        std::cout << "[HUDDisplay] HUD received PLAYER_ADDSCORE event! Updating text...\n";
        //m_pScoreTextComponent->SetText("# score: " + std::to_string(currentScore));

        std::ostringstream ss;
        ss << std::setfill('0') << std::setw(5) << currentScore;

        m_pScoreTextComponent->SetText(ss.str());
    }
    else if(event == EventId::PlAYER_HIT)
    {
        std::cout << "[HUDDisplay] HUD received PLAYER_HIT event!\n";
    }
}
