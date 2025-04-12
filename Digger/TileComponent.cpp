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
    case TileVisualType::Dug_Bottom: 
        texturePath = "Tiles/TileBottom.png"; 
        break;
    case TileVisualType::Dug_Up: 
        texturePath = "Tiles/TileUp.png"; 
        break;
    case TileVisualType::Dug_Left: 
        texturePath = "Tiles/TileLeft.png"; 
        break;
    case TileVisualType::Dug_Right: 
        texturePath = "Tiles/TileRight.png"; 
        break;
    case TileVisualType::Dug_Corner_BR: 
        texturePath = "Tiles/TileCornerBR.png"; 
        break;
    case TileVisualType::Dug_Corner_LB: 
        texturePath = "Tiles/TileCornerLB.png"; 
        break;
    case TileVisualType::Dug_Corner_LU: 
        texturePath = "Tiles/TileCornerLU.png"; 
        break;
    case TileVisualType::Dug_Corner_UR: 
        texturePath = "Tiles/TileCornerUR.png"; 
        break;
    case TileVisualType::Dug_TCross_LBR: 
        texturePath = "Tiles/TileTCrossLBR.png"; 
        break;
    case TileVisualType::Dug_TCross_LUR: 
        texturePath = "Tiles/TileTCrossLUR.png"; 
        break;
    case TileVisualType::Dug_TCross_ULB: 
        texturePath = "Tiles/TileTCrossULB.png"; 
        break;
    case TileVisualType::Dug_TCRoss_URB: 
        texturePath = "Tiles/TileTCrossURB.png"; 
        break;
    case TileVisualType::Dug_Vertical: 
        texturePath = "Tiles/TileVertical.png"; 
        break;
    case TileVisualType::Dug_Horizontal: 
        texturePath = "Tiles/TileHorizontal.png"; 
        break;
    case TileVisualType::Dug_Cross: 
        texturePath = "Tiles/TileBlack.png"; 
        break;
    }
}
