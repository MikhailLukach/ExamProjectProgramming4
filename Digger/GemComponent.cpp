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
#include <array>
#include <SoundServiceLocator.h>
#include <CollisionHelper.h>

const std::array<std::string, 8> dae::GemComponent::soundPaths = {
	"gem_pickup_1.wav",
	"gem_pickup_2.wav",
	"gem_pickup_3.wav",
	"gem_pickup_4.wav",
	"gem_pickup_5.wav",
	"gem_pickup_6.wav",
	"gem_pickup_7.wav",
	"gem_pickup_8.wav"
};

void dae::GemComponent::Render() const
{
	/*auto* owner = GetOwner();
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
	SDL_RenderDrawRect(renderer, &gemRect);*/
}

void dae::GemComponent::Update(float)
{
	auto* owner = GetOwner();
	if (!owner || !owner->GetComponent<RenderComponent>()) return;

	auto* scene = dae::SceneManager::GetInstance().GetCurrentScene();
	if (!scene) return;

	for (const auto& obj : scene->GetObjects())
	{
		if (obj.get() == owner) continue;

		if (!obj->GetComponent<RenderComponent>()) continue;
		if (!obj->GetComponent<ScoreComponent>()) continue;

		if (CheckRenderComponentCollision(owner, obj.get()))
		{
			auto score = obj->GetComponent<ScoreComponent>();
			auto gemTracker = obj->GetComponent<dae::GemTrackerComponent>();

			if (gemTracker)
			{
				int index = std::min(gemTracker->GetConsecutive(), 7);
				dae::SoundServiceLocator::Get().PlaySound(dae::ResourceManager::GetInstance().GetFullPath(soundPaths[index]));

				if (gemTracker->Collect())
				{
					score->AddPoints(250);
					std::cout << "[GemComponent] 8-in-a-row! special bonus!\n";
				}
				else
				{
					score->AddPoints(25);
				}
			}

			owner->MarkForDeletion();
			break;
		}
	}
}
