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
            if (++m_Consecutive >= 8)
            {
                m_Consecutive = 0;
                return true;
            }
            return false;
        }

        void Reset() { m_Consecutive = 0; }

    private:
        int m_Consecutive;
    };
}

