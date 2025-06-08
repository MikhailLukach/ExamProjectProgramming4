#include "GameObjectFactory.h"
#include "GameObject.h"

//Components
#include "RenderComponent.h"
#include "SpriteAnimatorComponent.h"
#include "TileTrackerComponent.h"
#include "LevelManagerComponent.h"
#include "MoneyBagComponent.h"
#include "GemComponent.h"

#include "IdleState.h"
#include "LevelLoader.h"
#include "Scene.h"
#include "GridSettings.h"

std::shared_ptr<dae::GameObject> dae::CreateMoneyBag(Scene& scene, LevelLoader& loader, int tileX, int tileY, TileManagerComponent* tileManager, LevelManagerComponent* levelManager)
{
	auto bagObj = std::make_shared<GameObject>();

	auto render = bagObj->AddComponent<RenderComponent>("WarningAppleSpritesheet.png");
	render->SetSize(32, 32);
	render->SetRenderOffset(glm::vec2{ 0.f, -16.f });

	auto animator = bagObj->AddComponent<SpriteAnimatorComponent>(render.get(), 16, 15, 0.2f);

	// Set idle source rect (frame 1)
	SDL_Rect idleRect{ 15, 0, 15, 15 };
	render->SetSourceRect(idleRect);

	auto tracker = bagObj->AddComponent<TileTrackerComponent>(
		GridSettings::TileWidth, GridSettings::TileHeight,
		GridSettings::GridOffsetX, GridSettings::GridOffsetY);
	tracker->SetTrackingMode(TrackingMode::TopLeft);

	auto moneyBag = bagObj->AddComponent<MoneyBagComponent>();
	moneyBag->SetState(std::make_unique<IdleState>());
	moneyBag->SetTileManager(tileManager);

	levelManager->RegisterMoneyBag(bagObj);

	auto pos = loader.GetWorldCenterForTile(tileX, tileY);
	bagObj->GetTransform()->SetPosition(pos);

	scene.Add(bagObj);

	return bagObj;
}

std::shared_ptr<dae::GameObject> dae::CreateGem(Scene& scene, LevelLoader& loader, int tileX, int tileY)
{
	auto gem = std::make_shared<dae::GameObject>();

	gem->GetTransform()->SetPosition(loader.GetWorldCenterForTile(tileX, tileY)); // example position

	auto gemRender = gem->AddComponent<dae::RenderComponent>("Cherry.png");
	gemRender->SetSize(32, 32);
	gemRender->SetRenderOffset(glm::vec2{ 0, -16 });

	gem->AddComponent<dae::GemComponent>();

	scene.Add(gem);

	return gem;
}
