/*#include "HealthComponent.h"

dae::HealthComponent::HealthComponent(int lives)
	: m_Lives(lives)
{

}

void dae::HealthComponent::TakeDamage(int damage)
{
    if (m_Lives > 0)
    {
        m_Lives -= damage;
        if (m_Lives <= 0)
        {
            std::cout << "[DEBUG] Player has died! Notifying observers...\n";
            m_Lives = 0; // Ensure health does not go negative
            NotifyObservers(EventId::PLAYER_DIED, this->GetOwner());
        }
        else
        {
            std::cout << "[DEBUG] Player took damage! Remaining health: " << m_Lives << "\n";
            NotifyObservers(EventId::PLAYER_DAMAGED, this->GetOwner());
        }
    }
}

int dae::HealthComponent::GetLives() const
{
    return m_Lives;
}*/
