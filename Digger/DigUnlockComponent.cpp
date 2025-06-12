#include "DigUnlockComponent.h"
#include "RenderComponent.h"
#include "GameObject.h"
#include "SpriteAnimatorComponent.h"

dae::DigUnlockComponent::DigUnlockComponent(float unlockAfterSecs, float digWindowSecs)
	: m_UnlockAfter(unlockAfterSecs)
	, m_DigWindow(digWindowSecs)
	, m_Timer(0.f)
	, m_CanDig(false)
{
}

void dae::DigUnlockComponent::Update(float deltaTime)
{
    m_Timer += deltaTime;

    if (!m_CanDig)
    {
        if (m_Timer >= m_UnlockAfter)
        {
            m_CanDig = true;
            auto render = GetOwner()->GetComponent<RenderComponent>();

            if (render)
            {
                render->SetTexture("HobbinSpriteSheetVer2.png"); 
                render->SetSize(32, 32);                     
            }

            auto animator = GetOwner()->GetComponent<SpriteAnimatorComponent>();
            if (animator)
            {
                animator->Configure(render.get(), 16, 16, 0.12f); 
                animator->PlayAnimation(6, 3); 
            }
            m_Timer = 0.f;
        }
    }
    else
    {
        if (m_Timer >= m_DigWindow)
        {
            m_CanDig = false;
            auto render = GetOwner()->GetComponent<RenderComponent>();

            if (render)
            {
                render->SetTexture("NormalNobbinSpritesheet.png"); 
                render->SetSize(32, 32);                     
            }

            auto animator = GetOwner()->GetComponent<SpriteAnimatorComponent>();
            if (animator)
            {
                animator->Configure(render.get(), 16, 16, 0.12f);
                animator->PlayAnimation(6, 3);
            }
            m_Timer = 0.f; 
        }
    }
}
