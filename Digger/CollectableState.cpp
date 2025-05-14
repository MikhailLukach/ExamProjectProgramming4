#include "CollectableState.h"
#include <iostream>
#include "GameObject.h"
#include "SpriteAnimatorComponent.h"
#include "MoneyBagComponent.h"

void dae::CollectableState::OnEnter(MoneyBagComponent& bag)
{
	std::cout << "[MoneyBag] Became collectable!\n";

	auto render = bag.GetOwner()->GetComponent<RenderComponent>();
	auto animator = bag.GetOwner()->GetComponent<SpriteAnimatorComponent>();

	if (animator)
	{
		animator->Stop(); // Stop any previous animation
	}

	if (render)
	{
		// Set the gem sprite
		render->SetTexture("Gem.png");        // Make sure this asset exists
		render->SetSize(32, 32);              // Adjust if your gem is another size
		render->SetRenderOffset({ 0.f, -10.f }); // Center vertically if needed

		// Ensure we're not using an animation slice
		render->ClearSourceRect();
	}
}

std::unique_ptr<dae::MoneyBagState> dae::CollectableState::Update(MoneyBagComponent& bag, float deltaTime)
{
	(void)bag;
	(void)deltaTime;
	//std::cout << "[MoneyBag] Updating collectable.\n";
	//detect collision with player
	return nullptr;
}

void dae::CollectableState::OnExit(MoneyBagComponent& bag)
{
	(void)bag;
	std::cout << "[MoneyBag] It's not collectable anymore!\n";
}
