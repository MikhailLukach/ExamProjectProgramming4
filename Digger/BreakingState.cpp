#include "BreakingState.h"
#include "MoneyBagComponent.h"
#include <iostream>
#include "CollectableState.h"

void dae::BreakingState::OnEnter(MoneyBagComponent& bag)
{
    (void)bag;
	std::cout << "[MoneyBag] Breaking open!\n";
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
