#include "HUDDisplay.h"
#include "Rendercomponent.h"
#include "GameObject.h"
#include <string>
#include <iomanip>
#include <sstream> 

dae::HUDDisplay::HUDDisplay(TextComponent* scoreText, ScoreComponent* score)
    : m_pScoreTextComponent(scoreText), m_pScoreComponent(score)
{
    std::ostringstream ss;
    ss << std::setfill('0') << std::setw(5) << m_pScoreComponent->GetScore();

    m_pScoreTextComponent->SetText(ss.str());
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
        if (!m_VisibleLifeIcons.empty())
        {
            GameObject* icon = m_VisibleLifeIcons.back();
            if (auto render = icon->GetComponent<RenderComponent>())
            {
                render->SetVisible(false);
            }
            m_VisibleLifeIcons.pop_back();
        }
    }
    else if (event == EventId::PLAYER_GAINEDLIVES)
    {
        std::cout << "[HUDDisplay] HUD received PLAYER_GAINEDLIVES event!\n";

        if (m_VisibleLifeIcons.size() < m_AllLifeIcons.size())
        {
            GameObject* iconToShow = m_AllLifeIcons[m_VisibleLifeIcons.size()];

            if (auto render = iconToShow->GetComponent<RenderComponent>())
            {
                render->SetVisible(true);
            }
            m_VisibleLifeIcons.push_back(iconToShow);
        }
    }
}

void dae::HUDDisplay::SetLifeIcons(const std::vector<GameObject*>& icons)
{
    m_AllLifeIcons = icons;  // full set
    m_VisibleLifeIcons = icons;  // all start out visible
}
