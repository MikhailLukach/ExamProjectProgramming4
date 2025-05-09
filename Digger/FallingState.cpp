#include "FallingState.h"
#include "MoneyBagComponent.h"
#include <iostream>
#include "BreakingState.h"
#include "IdleState.h"

void dae::FallingState::OnEnter(MoneyBagComponent& bag)
{
	std::cout << "[MoneyBag] Entering FallingState\n";
	bag.ResetFallDistance();
}

std::unique_ptr<dae::MoneyBagState> dae::FallingState::Update(MoneyBagComponent& bag, float deltaTime)
{
    m_FallTimer += deltaTime;
    std::cout << "[MoneyBag] It's falling!\n";
    if (m_FallTimer >= 0.5f)
    {
        m_FallTimer = 0.0f;
        bag.AddFallDistance(1);
        std::cout << "[MoneyBag] Falling... Total tiles fallen: " << bag.GetFallDistance() << "\n";

        if (false) //check for ground below
        {
            if (bag.GetFallDistance() >= 2)
            {
                return std::make_unique<BreakingState>();
            }
            return std::make_unique<IdleState>();
        }
    }

    return nullptr;
}

void dae::FallingState::OnExit(MoneyBagComponent& bag)
{
    (void)bag;
    std::cout << "[MoneyBag] Exited FallingState\n";
}
