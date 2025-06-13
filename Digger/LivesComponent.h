#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class LivesComponent : public Component, public Subject
	{
    public:
        LivesComponent(int lives, int playerId = 1) 
            : m_Lives(lives)
            , m_PlayerId(playerId)
        {}

        void LoseLife();

        void AddLife();

        int GetLives() const { return m_Lives; }

        int GetPlayerId() const { return m_PlayerId; }

    private:
        int m_Lives;
        int m_PlayerId;
	};
}

