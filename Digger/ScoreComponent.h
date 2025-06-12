#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class ScoreComponent : public Component, public Subject
	{
    public:
        explicit ScoreComponent(int score);
        void AddPoints(int points);
        int GetScore() const;

        int  m_NextExtraLifeScore = 20000;
    private:
        int m_Score;
	};
}

