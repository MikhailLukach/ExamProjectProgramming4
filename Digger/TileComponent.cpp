#include "TileComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Transform.h"

dae::TileComponent::TileComponent(TileVisualType type)
	:m_Type(type)
{

}

void dae::TileComponent::SetType(TileVisualType newType)
{
	m_Type = newType;
	UpdateTexture();
}

dae::TileVisualType dae::TileComponent::GetType() const
{
	return m_Type;
}

void dae::TileComponent::SetRenderComponent(RenderComponent* renderComponent)
{
	m_pRenderComponent = renderComponent;
	UpdateTexture();
}

void dae::TileComponent::UpdateTexture()
{
	if(!m_pRenderComponent)
	{
		return;
	}

    std::string texturePath;

    switch (m_Type)
    {
    case TileVisualType::Undug: 
        texturePath = "Tiles/TileFilled.png"; 
        break;
    case TileVisualType::Dug_Spot: 
        texturePath = "Tiles/TileEmpty.png"; 
        break;
    }
}
