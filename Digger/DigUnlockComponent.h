#pragma once
#include "Component.h"

namespace dae 
{
    class DigUnlockComponent : public Component
    {
    public:
        DigUnlockComponent(float unlockAfterSecs = 60.f,
            float digWindowSecs = 15.f);

        void Update(float deltaTime) override;

        bool CanDig() const { return m_CanDig; }

    private:
        float m_UnlockAfter;    
        float m_DigWindow;      
        float m_Timer;         

        bool  m_CanDig;         
    };
}

