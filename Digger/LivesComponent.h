#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
	class LivesComponent : public Component, public Subject
	{
    public:
        LivesComponent(int lives) : m_Lives(lives) {}

        void LoseLife();

        void AddLife();

        int GetLives() const { return m_Lives; }

    private:
        int m_Lives;
	};
}

