#pragma once
#include "Component.h"
#include "TileTypes.h"
#include <string>

namespace dae
{
	class RenderComponent;

	class TileComponent : public Component
	{
	public:
		TileComponent(TileVisualType type);

		void SetType(TileVisualType newType);
		TileVisualType GetType() const;

		void SetRenderComponent(RenderComponent* renderComponent);
		void UpdateTexture();

		void Dig();
	private:
		TileVisualType m_Type;
		RenderComponent* m_pRenderComponent{};
	};
}

