#include "LivesComponent.h"
#include "PersistentPlayerStats.h"

void dae::LivesComponent::LoseLife()
{
    if (--m_Lives <= -1)
    {
        if (m_PlayerId == 1)
        {
            dae::g_Player1Stats.Lives = 3;
            dae::g_Player1Stats.Score = 0;
        }
        else if (m_PlayerId == 2)
        {
            dae::g_Player2Stats.Lives = 3;
            dae::g_Player2Stats.Score = 0;
        }

        NotifyObservers(EventId::PLAYER_DIED, GetOwner());
    }
    else
    {
        NotifyObservers(EventId::PlAYER_HIT, GetOwner());
    }
}

void dae::LivesComponent::AddLife()
{
    ++m_Lives;
    NotifyObservers(EventId::PLAYER_GAINEDLIVES, GetOwner());
}
