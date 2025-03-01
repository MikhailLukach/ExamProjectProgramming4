#include "FPSComponent.h"
#include <iostream>

dae::FPSComponent::FPSComponent(std::shared_ptr<TextComponent> textComponent)
	:m_pTextComponent(textComponent)
{
}

void dae::FPSComponent::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;
	m_FrameCount++;
    if (m_ElapsedTime >= 1.0f)
    {
        int fps = static_cast<int>(m_FrameCount / m_ElapsedTime);
        if(auto textComp = m_pTextComponent.lock())
        {
            textComp->SetText("FPS: " + std::to_string(fps));
        }

        m_ElapsedTime = 0.0f;
        m_FrameCount = 0;
    }
}
