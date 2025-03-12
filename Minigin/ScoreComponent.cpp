#include "ScoreComponent.h"

dae::ScoreComponent::ScoreComponent(int score)
	:m_Score(score)
{
}

void dae::ScoreComponent::AddPoints(int points)
{
	std::cout << "[DEBUG] Player gains points! Notifying observers...\n";
	m_Score += points;
	NotifyObservers(EventId::PLAYER_ADDSCORE, this->GetOwner());
}

int dae::ScoreComponent::GetScore() const
{
	return m_Score;
}
