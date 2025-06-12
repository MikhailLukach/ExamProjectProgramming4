#include "ScoreComponent.h"
#include "LivesComponent.h"
#include "GameObject.h"

dae::ScoreComponent::ScoreComponent(int score)
	:m_Score(score)
{
}

void dae::ScoreComponent::AddPoints(int points)
{
	std::cout << "[DEBUG] Player gains points! Notifying observers...\n";
	//int oldScore = m_Score;
	m_Score += points;
	
	// update HUD score text
	NotifyObservers(EventId::PLAYER_ADDSCORE, GetOwner());
	
		   // for every 20000-point boundary we cross, grant an extra life
	while (m_Score >= m_NextExtraLifeScore)
	{
	       // find player's LivesComponent and give one life
		if (auto owner = GetOwner())
		{
			if (auto lives = owner->GetComponent<LivesComponent>())
			{
				std::cout << "[DEBUG] Score crossed " << m_NextExtraLifeScore << ", awarding extra life\n";
				lives->AddLife();
			}
		}
		m_NextExtraLifeScore += 20000;
	}
}

int dae::ScoreComponent::GetScore() const
{
	return m_Score;
}
