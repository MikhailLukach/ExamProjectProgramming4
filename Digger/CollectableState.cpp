#include "CollectableState.h"
#include <iostream>
#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SpriteAnimatorComponent.h"
#include "MoneyBagComponent.h"
#include "ScoreComponent.h"

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

	auto bagObj = bag.GetOwner();
	auto bagTransform = bagObj->GetTransform();
	auto bagRender = bagObj->GetComponent<RenderComponent>();

	if (!bagRender) return nullptr;

	glm::vec3 bagPos = bagTransform->GetWorldPosition();
	float bagWidth = 32.f;  // consistent with SetSize in OnEnter
	float bagHeight = 32.f;

	// Simple AABB for the bag
	SDL_Rect bagRect{
		static_cast<int>(bagPos.x),
		static_cast<int>(bagPos.y),
		static_cast<int>(bagWidth),
		static_cast<int>(bagHeight)
	};

	// Look through all GameObjects in the scene
	auto* scene = dae::SceneManager::GetInstance().GetCurrentScene(); // optionally add a getter

	if (!scene) return nullptr;

	for (auto& obj : scene->GetObjects()) // you may need to expose GetObjects()
	{
		if (obj.get() == bagObj) continue;

		auto scoreComp = obj->GetComponent<ScoreComponent>();
		if (!scoreComp) continue; // Not a player

		auto playerTransform = obj->GetTransform();
		glm::vec3 playerPos = playerTransform->GetWorldPosition();

		// Assuming player has similar size
		SDL_Rect playerRect{
			static_cast<int>(playerPos.x),
			static_cast<int>(playerPos.y),
			32, 32
		};

		// AABB collision
		bool isOverlapping = SDL_HasIntersection(&bagRect, &playerRect);
		if (isOverlapping)
		{
			//std::cout << "[CollectableState] Player overlapped with collectable!\n";
			scoreComp->AddPoints(25);
			bag.GetOwner()->MarkForDeletion();
		}
	}

	return nullptr;
}

void dae::CollectableState::OnExit(MoneyBagComponent& bag)
{
	(void)bag;
	std::cout << "[MoneyBag] It's not collectable anymore!\n";
}
