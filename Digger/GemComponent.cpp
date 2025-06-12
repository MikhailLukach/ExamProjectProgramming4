#include "GemComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "ScoreComponent.h"
#include "RenderComponent.h"
#include "GemTrackerComponent.h"
#include "SceneManager.h"
#include "Scene.h"

#include <SDL.h>
#include <iostream>

void dae::GemComponent::Render() const
{
	auto* owner = GetOwner();
	if (!owner) return;

	auto transform = owner->GetTransform();
	if (!transform) return;

	glm::vec3 gemPos = transform->GetWorldPosition();

	SDL_Rect gemRect{
		static_cast<int>(gemPos.x),
		static_cast<int>(gemPos.y - 16),
		32, 32 // match the size you used for collision
	};

	SDL_Renderer* renderer = dae::Renderer::GetInstance().GetSDLRenderer();
	if (!renderer) return;

	// Red debug box
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(renderer, &gemRect);
}

void dae::GemComponent::Update(float)
{
	auto* owner = GetOwner();
	auto transform = owner->GetTransform();
	auto render = owner->GetComponent<RenderComponent>();
	if (!render) return;

	glm::vec3 gemPos = transform->GetWorldPosition();
	SDL_Rect gemRect{ static_cast<int>(gemPos.x), static_cast<int>(gemPos.y - 16), 32, 32 };

	auto* scene = dae::SceneManager::GetInstance().GetCurrentScene();
	if (!scene) return;

	for (const auto& obj : scene->GetObjects())
	{
		if (obj.get() == owner) continue;

		auto score = obj->GetComponent<ScoreComponent>();
		if (!score) continue;

		auto playerTransform = obj->GetTransform();
		SDL_Rect playerRect{ static_cast<int>(playerTransform->GetWorldPosition().x), static_cast<int>(playerTransform->GetWorldPosition().y), 32, 32 };

		if (SDL_HasIntersection(&gemRect, &playerRect))
		{
			std::cout << "[GemComponent] Player collected a gem!\n";
			auto gemTracker = obj->GetComponent<dae::GemTrackerComponent>();
			if (gemTracker && gemTracker->Collect())
			{
				score->AddPoints(250);
				std::cout << "[GemComponent] 8-in-a-row! special bonus!\n";
			}
			else
			{
				score->AddPoints(25);
			}
			owner->MarkForDeletion();
			break;
		}
	}
}
