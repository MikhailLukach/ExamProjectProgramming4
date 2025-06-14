#include "GridOutlineComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"

dae::GridOutlineComponent::GridOutlineComponent(int tileWidth, int tileHeight, SDL_Color color)
	:m_TileWidth(tileWidth), m_TileHeight(tileHeight), m_Color(color)
{

}

void dae::GridOutlineComponent::Render() const
{
	/*if (!GetOwner()) return;

	auto pos = GetOwner()->GetTransform()->GetLocalPosition();
	SDL_Rect rect{};
	rect.x = static_cast<int>(pos.x);
	rect.y = static_cast<int>(pos.y);
	rect.w = m_TileWidth;
	rect.h = m_TileHeight;

	SDL_Renderer* renderer = Renderer::GetInstance().GetSDLRenderer();
	if (!renderer) return;

	SDL_SetRenderDrawColor(renderer, m_Color.r, m_Color.g, m_Color.b, m_Color.a);
	SDL_RenderDrawRect(renderer, &rect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Reset to white*/
}
