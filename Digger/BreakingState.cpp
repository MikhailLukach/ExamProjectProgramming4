#include "BreakingState.h"
#include "MoneyBagComponent.h"
#include <iostream>
#include "CollectableState.h"
#include "GameObject.h"
#include "SpriteAnimatorComponent.h"
#include <SoundServiceLocator.h>

void dae::BreakingState::OnEnter(MoneyBagComponent& bag)
{
    std::cout << "[MoneyBag] Breaking open!\n";

    auto render = bag.GetOwner()->GetComponent<RenderComponent>();
    auto animator = bag.GetOwner()->GetComponent<SpriteAnimatorComponent>();

    if (render && animator)
    {
        render->SetTexture("BreakingAppleSpritesheetVer2.png");
        render->SetSize(64, 32);
        render->SetRenderOffset(glm::vec2{ -16, -16 });

        animator->Configure(render.get(), 34, 15, 0.15f);

        animator->PlayAnimation(0, 3, false);  
    }

    dae::SoundServiceLocator::Get().PlaySound(dae::ResourceManager::GetInstance().GetFullPath("AppleBreaks.wav"));
}

std::unique_ptr<dae::MoneyBagState> dae::BreakingState::Update(MoneyBagComponent& bag, float deltaTime)
{
    (void)bag;

    std::cout << "[MoneyBag] Updating Breaking.\n";

    m_BreakTime += deltaTime;

    if (m_BreakTime >= 1.0f)
    {
        return std::make_unique<CollectableState>();
    }

    return nullptr;
}

void dae::BreakingState::OnExit(MoneyBagComponent& bag)
{
    (void)bag;
    std::cout << "[MoneyBag] Exited Breaking!\n";
}
