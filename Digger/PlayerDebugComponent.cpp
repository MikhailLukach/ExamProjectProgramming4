#include "PlayerDebugComponent.h"
#include "TileTrackerComponent.h"
#include "GameObject.h"
#include "Transform.h"
#include "Renderer.h"
#include <SDL.h>

void dae::PlayerDebugComponent::Render() const
{
	auto owner = GetOwner();
	if (!owner) return;

	auto transform = owner->GetTransform();
	if (!transform) return;

	glm::vec3 pos = transform->GetWorldPosition();

	SDL_Rect playerRect{
		static_cast<int>(pos.x),
		static_cast<int>(pos.y),
		32, 32  // match your player size
	};

	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();
	if (!renderer) return;

	SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255); // Green
	SDL_RenderDrawRect(renderer, &playerRect);

	auto tracker = owner->GetComponent<TileTrackerComponent>();
	if (tracker)
	{
		glm::ivec2 tile = tracker->GetTileCoords();
		const int tileW = 42; // or GridSettings::TileWidth
		const int tileH = 43;
		const int offsetX = 5;  // Or GridSettings::GridOffsetX
		const int offsetY = 48;

		SDL_Rect tileRect{
			offsetX + tile.x * tileW,
			offsetY + tile.y * tileH,
			tileW,
			tileH
		};

		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255); // Yellow tile overlay
		SDL_RenderDrawRect(renderer, &tileRect);
	}
}