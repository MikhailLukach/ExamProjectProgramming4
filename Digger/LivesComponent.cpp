#include "LivesComponent.h"
#include "PersistentPlayerStats.h"

void dae::LivesComponent::LoseLife()
{
    if (--m_Lives <= -1)
    {
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
