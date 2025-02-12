#include "FPSComponent.h"
#include <iostream>

dae::FPSComponent::FPSComponent(std::shared_ptr<TextComponent> textComponent)
	:m_pTextComponent(std::move(textComponent))
{
}

void dae::FPSComponent::Update(float deltaTime)
{
	m_ElapsedTime += deltaTime;
	m_FrameCount++;
    if (m_ElapsedTime >= 1.0f)
    {
        int fps = static_cast<int>(m_FrameCount / m_ElapsedTime);
        if(m_pTextComponent)
        {
            m_pTextComponent->SetText("FPS: " + std::to_string(fps));
        }

        m_ElapsedTime = 0.0f;
        m_FrameCount = 0;
    }
}
