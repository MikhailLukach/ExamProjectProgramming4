#pragma once
#include "Component.h"

namespace dae
{
	class GemTrackerComponent : public Component
	{
    public:
        GemTrackerComponent() : m_Consecutive(0) {}

        bool Collect()
        {
            m_Timer = 0.f;
            if (++m_Consecutive >= 8)
            {
                m_Consecutive = 0;
                return true;
            }
            return false;
        }

        void Update(float deltaTime) override
        {
            m_Timer += deltaTime;
            if (m_Timer >= m_Timeout)
            {
                m_Consecutive = 0;
                m_Timer = 0.f;
            }
        }

        void Reset() { m_Consecutive = 0; m_Timer = 0.f; }

        int GetConsecutive() const { return m_Consecutive; }

    private:
        int m_Consecutive;
        float m_Timer = 0.f;
        const float m_Timeout{ 2.f }; // timeout duration in seconds
    };
}

