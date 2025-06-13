#include "ScoreComponent.h"
#include "LivesComponent.h"
#include "GameObject.h"
#include "PersistentPlayerStats.h"

dae::ScoreComponent::ScoreComponent(int score, int playerId)
	:m_Score(score)
	, m_Player_Id(playerId)
{
}

void dae::ScoreComponent::AddPoints(int points)
{
	std::cout << "[DEBUG] Player gains points! Notifying observers...\n";
	//int oldScore = m_Score;
	m_Score += points;

	if (m_Player_Id == 1)
		dae::g_Player1Stats.Score = m_Score;
	else if (m_Player_Id == 2)
		dae::g_Player2Stats.Score = m_Score;
	
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
	if (m_Player_Id == 1)
	{
		std::cout << "[DEBUG] GetScore called. Player " << m_Player_Id << " has score " << m_Score << " (stored: "
			<< (m_Player_Id == 1 ? dae::g_Player1Stats.Score : dae::g_Player2Stats.Score) << ")\n";
		return dae::g_Player1Stats.Score;
	}
	else if (m_Player_Id == 2)
	{
		std::cout << "[DEBUG] GetScore called. Player " << m_Player_Id << " has score " << m_Score << " (stored: "
			<< (m_Player_Id == 1 ? dae::g_Player1Stats.Score : dae::g_Player2Stats.Score) << ")\n";
		return dae::g_Player2Stats.Score;
	}

	std::cout << "[DEBUG] GetScore called. Player " << m_Player_Id << " has score " << m_Score << " (stored: "
		<< (m_Player_Id == 1 ? dae::g_Player1Stats.Score : dae::g_Player2Stats.Score) << ")\n";

	return m_Score;
}
