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

	if (m_Score >= 500)
	{
		std::cout << "[DEBUG] Player reached 500 points! Notifying observers...\n";
		NotifyObservers(EventId::PLAYER_REACHED_500_POINTS, GetOwner());
	}
}

int dae::ScoreComponent::GetScore() const
{
	return m_Score;
}
