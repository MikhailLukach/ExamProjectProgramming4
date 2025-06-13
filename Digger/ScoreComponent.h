#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class ScoreComponent : public Component, public Subject
	{
    public:
        explicit ScoreComponent(int score, int playerId);
        void AddPoints(int points);
        int GetScore() const;

        void SetScore(int score)
        {
            m_Score = score;
        }

        int  m_NextExtraLifeScore = 20000;
    private:
        int m_Score;

        int m_Player_Id;
	};
}

