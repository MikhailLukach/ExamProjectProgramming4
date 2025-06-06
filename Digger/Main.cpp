#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"
#include "InputManager.h"
#include "Command.h"
#include "HUDDisplay.h"

//components
#include "RotatorComponent.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "SpriteAnimatorComponent.h"
#include "TileTrackerComponent.h"
#include "TileManagerComponent.h"
#include "TileComponent.h"
#include "MoneyBagComponent.h"
#include "LevelManagerComponent.h"
#include "GemComponent.h"
#include "NobbinControllerComponent.h"
#include "GridOutlineComponent.h"
#include "NobbinComponent.h"

//states
#include "NobbinState.h"
#include "IdleState.h"
#include "FallingState.h"
#include "CollectableState.h"
#include "BreakingState.h"

#include "GameCommands.h"
#include "LevelLoader.h"
#include "SoundServiceLocator.h"
#include "SDLSoundSystem.h"
#include "GameObjectFactory.h"

//exercise Game Scene
/*
void load() 
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	dae::ResourceManager::GetInstance().Init("../Data");

	auto& input = dae::InputManager::GetInstance();

	const float char1Speed{ 5.f };
	const float char2Speed{ 10.f };

	input.BindCommandKeyboard(SDLK_w, dae::InputType::Pressed, std::make_unique<dae::JumpCommand>());
	input.BindCommandKeyboard(SDLK_SPACE, dae::InputType::Down, std::make_unique<dae::FireCommand>());

	input.BindCommandController(0, XINPUT_GAMEPAD_DPAD_UP, dae::InputType::Down, std::make_unique<dae::JumpCommand>());
	input.BindCommandController(0, XINPUT_GAMEPAD_DPAD_DOWN, dae::InputType::Pressed, std::make_unique<dae::FireCommand>());

	std::cout << "input commands have been set\n";

	auto go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetPosition(0, 0, 0);
	go->AddComponent<dae::RenderComponent>("background.tga");
	scene.Add(go);

	auto go2 = std::make_shared<dae::GameObject>();
	go2->GetTransform()->SetPosition(225, 125, 0);
	go2->AddComponent<dae::RenderComponent>("logo.tga");
	scene.Add(go2);

	auto go3 = std::make_shared<dae::GameObject>();
	go3->GetTransform()->SetPosition(175, 75, 0);
	go3->AddComponent<dae::TextComponent>("Programming 4 Assignment", "Lingua.otf", 24);
	scene.Add(go3);

	auto go4 = std::make_shared<dae::GameObject>();
	go4->GetTransform()->SetPosition(0, 0, 0);
	go4->AddComponent<dae::TextComponent>("FPS: 0", "Lingua.otf", 24);
	go4->AddComponent<dae::FPSComponent>(go4->GetComponent<dae::TextComponent>());
	scene.Add(go4);

	auto go5 = std::make_shared<dae::GameObject>();
	go5->GetTransform()->SetPosition(300, 300, 0);
	go5->AddComponent<dae::RenderComponent>("Mr!DoClown.png");
	go5->AddComponent<dae::HealthComponent>(3);
	go5->AddComponent<dae::ScoreComponent>(0);
	scene.Add(go5);

	auto HUDObjectP1 = std::make_shared<dae::GameObject>();
	HUDObjectP1->GetTransform()->SetPosition(10, 50, 0);

	auto p1HealthDisplay = std::make_shared<dae::GameObject>();
	auto p1HealthText = p1HealthDisplay->AddComponent<dae::TextComponent>("# lives: 3", "Lingua.otf", 24);
	p1HealthDisplay->GetTransform()->SetPosition(0, 50, 0);
	p1HealthDisplay->SetParent(HUDObjectP1, false);

	auto p1ScoreDisplay = std::make_shared<dae::GameObject>();
	auto p1ScoreText = p1ScoreDisplay->AddComponent<dae::TextComponent>("# score: 0", "Lingua.otf", 24);
	p1ScoreDisplay->GetTransform()->SetPosition(0, 75, 0);
	p1ScoreDisplay->SetParent(HUDObjectP1, false);

	auto p1HUD = HUDObjectP1->AddComponent<dae::HUDDisplay>
		(
			p1HealthText.get(),
			p1ScoreText.get(),
			go5->GetComponent<dae::HealthComponent>().get(),
			go5->GetComponent<dae::ScoreComponent>().get()
		);

	go5->GetComponent<dae::HealthComponent>()->AddObserver(p1HUD);
	go5->GetComponent<dae::ScoreComponent>()->AddObserver(p1HUD);

	scene.Add(HUDObjectP1);
	scene.Add(p1HealthDisplay);
	scene.Add(p1ScoreDisplay);

	auto go6 = std::make_shared<dae::GameObject>();
	go6->GetTransform()->SetPosition(300, 350, 0);
	go6->AddComponent<dae::RenderComponent>("MrDo!Enemy.png");
	go6->AddComponent<dae::HealthComponent>(3);
	go6->AddComponent<dae::ScoreComponent>(0);
	scene.Add(go6);

	auto HUDObjectP2 = std::make_shared<dae::GameObject>();
	HUDObjectP2->GetTransform()->SetPosition(400, 50, 0);

	auto p2HealthDisplay = std::make_shared<dae::GameObject>();
	auto p2HealthText = p2HealthDisplay->AddComponent<dae::TextComponent>("# lives: 3", "Lingua.otf", 24);
	p2HealthDisplay->GetTransform()->SetPosition(0, 100, 0);
	p2HealthDisplay->SetParent(HUDObjectP2, false);

	auto p2ScoreDisplay = std::make_shared<dae::GameObject>();
	auto p2ScoreText = p2ScoreDisplay->AddComponent<dae::TextComponent>("# score: 0", "Lingua.otf", 24);
	p2ScoreDisplay->GetTransform()->SetPosition(0, 125, 0);
	p2ScoreDisplay->SetParent(HUDObjectP2, false);

	auto p2HUD = HUDObjectP2->AddComponent<dae::HUDDisplay>(
		p2HealthText.get(),
		p2ScoreText.get(),
		go6->GetComponent<dae::HealthComponent>().get(),
		go6->GetComponent<dae::ScoreComponent>().get()
	);

	go6->GetComponent<dae::HealthComponent>()->AddObserver(p2HUD);
	go6->GetComponent<dae::ScoreComponent>()->AddObserver(p2HUD);

	scene.Add(HUDObjectP2);
	scene.Add(p2HealthDisplay);
	scene.Add(p2ScoreDisplay);

	auto go7 = std::make_shared<dae::GameObject>();
	go7->GetTransform()->SetPosition(0, 25, 0);
	go7->AddComponent<dae::TextComponent>("use WASD to move player 1, X to inflict damage, E to pick up pellets", "Lingua.otf", 15);
	scene.Add(go7);

	auto go8 = std::make_shared<dae::GameObject>();
	go8->GetTransform()->SetPosition(0, 40, 0);
	go8->AddComponent<dae::TextComponent>("use D-Pad to move player 2, C to inflict damage, R to pick up pellets", "Lingua.otf", 15);
	scene.Add(go8);

	//input for gameobject go5 and go6
	input.BindCommandKeyboard(SDLK_w, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(go5.get(), glm::vec3(0, -1, 0), char1Speed));
	input.BindCommandKeyboard(SDLK_s, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(go5.get(), glm::vec3(0, 1, 0), char1Speed));
	input.BindCommandKeyboard(SDLK_a, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(go5.get(), glm::vec3(-1, 0, 0), char1Speed));
	input.BindCommandKeyboard(SDLK_d, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(go5.get(), glm::vec3(1, 0, 0), char1Speed));

	input.BindCommandKeyboard(SDLK_x, dae::InputType::Down
		, std::make_unique<dae::KillPlayerCommand>(go5->GetComponent<dae::HealthComponent>().get()));
	input.BindCommandKeyboard(SDLK_e, dae::InputType::Down
		, std::make_unique<dae::AddScoreCommand>(go5->GetComponent<dae::ScoreComponent>().get(), 100));

	input.BindCommandController(0, dae::GameController::DPAD_UP, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(go6.get(), glm::vec3(0, -1, 0), char2Speed));
	input.BindCommandController(0, dae::GameController::DPAD_DOWN, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(go6.get(), glm::vec3(0, 1, 0), char2Speed));
	input.BindCommandController(0, dae::GameController::DPAD_LEFT, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(go6.get(), glm::vec3(-1, 0, 0), char2Speed));
	input.BindCommandController(0, dae::GameController::DPAD_RIGHT, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(go6.get(), glm::vec3(1, 0, 0), char2Speed));

	input.BindCommandKeyboard(SDLK_c, dae::InputType::Down
		, std::make_unique<dae::KillPlayerCommand>(go6->GetComponent<dae::HealthComponent>().get()));
	input.BindCommandKeyboard(SDLK_r, dae::InputType::Down
		, std::make_unique<dae::AddScoreCommand>(go6->GetComponent<dae::ScoreComponent>().get(), 100));

	//auto go5 = std::make_shared<dae::GameObject>();
	//go5->AddComponent<dae::CacheTestComponent>();
	//scene.Add(go5);

	//parent and child
	auto mainGameObject = std::make_shared<dae::GameObject>();
	mainGameObject->GetTransform()->SetPosition(300, 300, 0);
	mainGameObject->AddComponent<dae::RenderComponent>("Mr!DoClown.png");
	mainGameObject->AddComponent<dae::RotatorComponent>(50.0f, 90.0f, glm::vec3(300, 300, 0));

	auto childGameObject = std::make_shared<dae::GameObject>();
	childGameObject->GetTransform()->SetPosition(300, 350, 0);
	childGameObject->AddComponent<dae::RenderComponent>("MrDo!Enemy.png");
	childGameObject->AddComponent<dae::RotatorComponent>(50.0f, 90.0f, glm::vec3(350, 350, 0));

	childGameObject->SetParent(mainGameObject, true);

	if (childGameObject->GetParent() == mainGameObject)
	{
		std::cout << "Orbiting Character is correctly parented.\n";
	}

	scene.Add(mainGameObject);
	scene.Add(childGameObject);

	//testing stuff
	if (go->GetComponent<dae::RenderComponent>()->GetOwner() == go.get())
	{
		std::cout << "GetOwner() correctly returns the GameObject!\n";
	}
	else
	{
		std::cerr << "Error: GetOwner() does not match the original GameObject!\n";
	}

	//std::cout << "X:" << childGameObject->GetComponent<dae::Transform>()->GetPosition().x << std::endl;
	//std::cout << "Y:" << childGameObject->GetComponent<dae::Transform>()->GetPosition().y << std::endl;
	if (go->HasComponent<dae::Transform>())
	{
		std::cout << "Transform component successfully added!\n";

		auto retrievedTransform = go->GetComponent<dae::Transform>();
		glm::vec3 position = retrievedTransform->GetPosition();

		std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
	}
	else
	{
		std::cout << "Error: Transform component not found!\n";
	}
}
*/


void LoadGame()
{
	//-- Initial Setup
	auto& scene = dae::SceneManager::GetInstance().CreateScene("DiggerLevel1");
	dae::ResourceManager::GetInstance().Init("../Data");

	auto soundSystem = new dae::SDLSoundSystem();

	dae::SoundServiceLocator::Provide(soundSystem);

	const float playerSpeed = 2.5f;

	auto& input = dae::InputManager::GetInstance();
	//--

	//-- Level Setup
	auto tileManagerObj = std::make_shared<dae::GameObject>();
	auto tileManager = tileManagerObj->AddComponent<dae::TileManagerComponent>();

	auto levelManagerObj = std::make_shared<dae::GameObject>();
	auto levelManager = levelManagerObj->AddComponent<dae::LevelManagerComponent>();
	scene.Add(levelManagerObj);

	std::vector<std::vector<std::shared_ptr<dae::GameObject>>> tileGrid;

	dae::LevelLoader loader;
	loader.LoadLevel(scene, tileGrid);

	tileManager->InitWithTileGrid(std::move(tileGrid));
	scene.Add(tileManagerObj);
	//--

	//-- Player Setup
	auto player = std::make_shared<dae::GameObject>();
	int spawnX = 0;
	int spawnY = 0;
	auto spawnPos = loader.GetWorldCenterForTile(spawnX, spawnY);
	player->GetTransform()->SetPosition(spawnPos);

	auto score = player->AddComponent<dae::ScoreComponent>(0);

	auto render = player->AddComponent<dae::RenderComponent>("CharacterSpriteSheetFilledBackground.png");
	render->SetSize(32, 32);

	auto animator = player->AddComponent<dae::SpriteAnimatorComponent>(render.get(), 16, 16, 0.2f);

	animator->PlayAnimation(3, 3);

	constexpr int TileWidth = 42;
	constexpr int TileHeight = 43;
	constexpr int OffsetX = (640 - (TileWidth * 15)) / 2; // = 5
	constexpr int OffsetY = 48;

	player->AddComponent<dae::TileTrackerComponent>(TileWidth, TileHeight, OffsetX, OffsetY);

	scene.Add(player);
	//--

	//-- UI Setup
	/*auto infoDisplay = std::make_shared<dae::GameObject>();
	auto infoText = infoDisplay->AddComponent<dae::TextComponent>("Press the A-button on a XBOX gamepad to play sound effect.", "Lingua.otf", 14);
	infoDisplay->GetTransform()->SetPosition(0, 10, 0);

	auto infoDisplay2 = std::make_shared<dae::GameObject>();
	auto infoText2 = infoDisplay2->AddComponent<dae::TextComponent>("Press the B-button to make the apple switch between its states.", "Lingua.otf", 14);
	infoDisplay2->GetTransform()->SetPosition(0, 25, 0);

	scene.Add(infoDisplay);
	scene.Add(infoDisplay2);*/

	auto HUDObject = std::make_shared<dae::GameObject>();
	HUDObject->GetTransform()->SetPosition(10, 50, 0);

	auto scoreDisplay = std::make_shared<dae::GameObject>();
	auto scoreText = scoreDisplay->AddComponent<dae::TextComponent>("# score: 0", "Lingua.otf", 24);
	scoreDisplay->GetTransform()->SetPosition(0, 10, 0);
	scoreDisplay->SetParent(HUDObject, false);

	auto hud = HUDObject->AddComponent<dae::HUDDisplay>(scoreText.get(), score.get());

	score->AddObserver(hud);

	scene.Add(HUDObject);
	scene.Add(scoreDisplay);
	//--

	//-- Enemies Setup
	auto nobbin = std::make_shared<dae::GameObject>();
	auto nobbinPos = loader.GetWorldCenterForTile(12, 0);
	nobbin->GetTransform()->SetPosition(nobbinPos);

	auto nobRender = nobbin->AddComponent<dae::RenderComponent>("NormalNobbinSpritesheet.png");
	nobRender->SetSize(32, 32);
	nobRender->SetRenderOffset(glm::vec2{ 0.f, -16.f });

	auto animatorNobbin = nobbin->AddComponent<dae::SpriteAnimatorComponent>(nobRender.get(), 16, 16, 0.15f);
	animatorNobbin->PlayAnimation(6, 3); // Start with left-facing animation as default

	nobbin->AddComponent<dae::TileTrackerComponent>(TileWidth, TileHeight, OffsetX, OffsetY);

	nobbin->AddComponent<dae::NobbinComponent>();
	nobbin->AddComponent<dae::NobbinControllerComponent>(player.get(), tileManager.get(), &loader, 0.05f, 100.f);

	scene.Add(nobbin);
	//--

	//-- Player Controller Setup
	input.BindCommandController(0, dae::GameController::DPAD_UP, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(player.get(), glm::vec3(0, -1, 0), playerSpeed, 
			 tileManager.get(), levelManager.get(), animator.get(), dae::AnimationState::WalkUp));
	input.BindCommandController(0, dae::GameController::DPAD_UP, dae::InputType::Released,
		std::make_unique<dae::StopAnimationCommand>(animator.get()));

	input.BindCommandController(0, dae::GameController::DPAD_DOWN, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(player.get(), glm::vec3(0, 1, 0), playerSpeed,
			tileManager.get(), levelManager.get(), animator.get(), dae::AnimationState::WalkDown));
	input.BindCommandController(0, dae::GameController::DPAD_DOWN, dae::InputType::Released,
		std::make_unique<dae::StopAnimationCommand>(animator.get()));

	input.BindCommandController(0, dae::GameController::DPAD_LEFT, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(player.get(), glm::vec3(-1, 0, 0), playerSpeed,
			 tileManager.get(), levelManager.get(), animator.get(), dae::AnimationState::WalkLeft));
	input.BindCommandController(0, dae::GameController::DPAD_LEFT, dae::InputType::Released,
		std::make_unique<dae::StopAnimationCommand>(animator.get()));

	input.BindCommandController(0, dae::GameController::DPAD_RIGHT, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(player.get(), glm::vec3(1, 0, 0), playerSpeed,
			 tileManager.get(), levelManager.get(), animator.get(), dae::AnimationState::WalkRight));
	input.BindCommandController(0, dae::GameController::DPAD_RIGHT, dae::InputType::Released,
		std::make_unique<dae::StopAnimationCommand>(animator.get()));

	input.BindCommandController(
		0,
		dae::GameController::A,
		dae::InputType::Released,
		std::make_unique<dae::PlaySoundCommand>(dae::ResourceManager::GetInstance().GetFullPath("Explosion Sound Effect.wav"))  // Replace with a valid file
	);
	//--

	//-- MoneyBag Setup
	dae::CreateMoneyBag(scene, loader, 1, 2, tileManager.get(), levelManager.get());
	dae::CreateMoneyBag(scene, loader, 4, 0, tileManager.get(), levelManager.get());
	dae::CreateMoneyBag(scene, loader, 5, 3, tileManager.get(), levelManager.get());
	dae::CreateMoneyBag(scene, loader, 8, 3, tileManager.get(), levelManager.get());
	dae::CreateMoneyBag(scene, loader, 8, 6, tileManager.get(), levelManager.get());
	dae::CreateMoneyBag(scene, loader, 12, 1, tileManager.get(), levelManager.get());
	dae::CreateMoneyBag(scene, loader, 6, 6, tileManager.get(), levelManager.get());
	//--

	//-- Gem Setup
	dae::CreateGem(scene, loader, 3, 1);
	dae::CreateGem(scene, loader, 3, 2);
	dae::CreateGem(scene, loader, 3, 3);
	dae::CreateGem(scene, loader, 3, 4);
	dae::CreateGem(scene, loader, 3, 5);

	dae::CreateGem(scene, loader, 4, 1);
	dae::CreateGem(scene, loader, 4, 2);
	dae::CreateGem(scene, loader, 4, 3);
	dae::CreateGem(scene, loader, 4, 4);
	dae::CreateGem(scene, loader, 4, 5);

	dae::CreateGem(scene, loader, 7, 1);
	dae::CreateGem(scene, loader, 7, 2);
	dae::CreateGem(scene, loader, 7, 3);
	dae::CreateGem(scene, loader, 7, 4);
	dae::CreateGem(scene, loader, 7, 5);

	dae::CreateGem(scene, loader, 12, 3);
	dae::CreateGem(scene, loader, 12, 4);
	dae::CreateGem(scene, loader, 12, 5);
	dae::CreateGem(scene, loader, 13, 3);
	dae::CreateGem(scene, loader, 13, 4);
	dae::CreateGem(scene, loader, 13, 5);
	dae::CreateGem(scene, loader, 14, 3);
	dae::CreateGem(scene, loader, 14, 4);
	dae::CreateGem(scene, loader, 14, 5);

	dae::CreateGem(scene, loader, 0, 8);
	dae::CreateGem(scene, loader, 0, 9);
	dae::CreateGem(scene, loader, 1, 9);

	dae::CreateGem(scene, loader, 14, 8);
	dae::CreateGem(scene, loader, 14, 9);
	dae::CreateGem(scene, loader, 13, 9);
	//--
}

int main(int, char* [])
{
	dae::Minigin engine("../Data/");
	//engine.Run(load);
	engine.Run(LoadGame);
	return 0;
}

