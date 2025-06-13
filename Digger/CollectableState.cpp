#include "CollectableState.h"
#include <iostream>
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SpriteAnimatorComponent.h"
#include "MoneyBagComponent.h"
#include "ScoreComponent.h"
#include <CollisionHelper.h>
#include <SoundServiceLocator.h>

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
	(void)deltaTime;
	auto* bagObj = bag.GetOwner();
	if (!bagObj) return nullptr;

	auto bagRender = bagObj->GetComponent<RenderComponent>();
	if (!bagRender) return nullptr;

	auto* scene = dae::SceneManager::GetInstance().GetCurrentScene();
	if (!scene) return nullptr;

	for (const auto& obj : scene->GetObjects())
	{
		if (!obj || obj.get() == bagObj) continue;

		auto scoreComp = obj->GetComponent<ScoreComponent>();
		if (!scoreComp) continue;

		if (!obj->GetComponent<RenderComponent>()) continue;

		if (CheckRenderComponentCollision(bagObj, obj.get()))
		{
			scoreComp->AddPoints(500);
			dae::SoundServiceLocator::Get().PlaySound(dae::ResourceManager::GetInstance().GetFullPath("ItemCollected.wav"));
			bagObj->MarkForDeletion();
			break;
		}
	}

	return nullptr;
}

void dae::CollectableState::OnExit(MoneyBagComponent& bag)
{
	(void)bag;
	std::cout << "[MoneyBag] It's not collectable anymore!\n";
}
