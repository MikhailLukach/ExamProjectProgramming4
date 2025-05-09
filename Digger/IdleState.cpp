#include "IdleState.h"
#include <iostream>
#include "FallingState.h"

void dae::IdleState::OnEnter(MoneyBagComponent& bag)
{
    (void)bag;
    std::cout << "[Debug] Entered IdleState\n";
}

std::unique_ptr<dae::MoneyBagState> dae::IdleState::Update(MoneyBagComponent& bag, float deltaTime)
{
    std::cout << "[Debug] IdleState is running\n";
    (void)bag;
    (void)deltaTime;
    if (false) //check for tile below
    {
        std::cout << "[MoneyBag] Tile below is dug. Transition to Falling.\n";
        return std::make_unique<FallingState>();
    }

    return nullptr;
}

void dae::IdleState::OnExit(MoneyBagComponent& bag)
{
    (void)bag;
    std::cout << "[Debug] Exiting IdleState\n";
}
