#include "CollectableState.h"
#include <iostream>

void dae::CollectableState::OnEnter(MoneyBagComponent& bag)
{
	(void)bag;
	std::cout << "[MoneyBag] Became collectable!\n";
}

std::unique_ptr<dae::MoneyBagState> dae::CollectableState::Update(MoneyBagComponent& bag, float deltaTime)
{
	(void)bag;
	(void)deltaTime;
	std::cout << "[MoneyBag] Updating collectable.\n";
	//detect collision with player
	return nullptr;
}

void dae::CollectableState::OnExit(MoneyBagComponent& bag)
{
	(void)bag;
	std::cout << "[MoneyBag] It's not collectable anymore!\n";
}
