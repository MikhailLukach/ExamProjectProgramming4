#include "LivesComponent.h"
#include "PersistentPlayerStats.h"
#include <SoundServiceLocator.h>
#include "ResourceManager.h"

void dae::LivesComponent::LoseLife()
{
    if (--m_Lives <= -1)
    {
        NotifyObservers(EventId::PLAYER_DIED, GetOwner());
    }
    else
    {
        dae::SoundServiceLocator::Get().PlaySound(dae::ResourceManager::GetInstance().GetFullPath("LoseaLife.wav"));
        NotifyObservers(EventId::PlAYER_HIT, GetOwner());
    }
}

void dae::LivesComponent::AddLife()
{
    ++m_Lives;
    NotifyObservers(EventId::PLAYER_GAINEDLIVES, GetOwner());
}
