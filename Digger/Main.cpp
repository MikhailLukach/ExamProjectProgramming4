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
#include "NobbinSpawnerComponent.h"
#include "DigUnlockComponent.h"
#include "GridOutlineComponent.h"
#include "NobbinComponent.h"
#include "PlayerDebugComponent.h"
#include "LivesComponent.h"
#include "PlayerRespawnComponent.h"
#include "LevelResetComponent.h"
#include "GemTrackerComponent.h"

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
#include "VersusDamageComponent.h"

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

void LoadScoreBoard()
{
	//-- Initial Game Setup
	//auto& scene = dae::SceneManager::GetInstance().CreateScene("Scoreboard");

	//auto& input = dae::InputManager::GetInstance();
	//--
}

void LoadGame(int levelIndex = 1)
{
	//-- Initial Game Setup
	auto& scene = dae::SceneManager::GetInstance().CreateScene("DiggerLevel1");

	const float playerSpeed = 2.f;

	auto& input = dae::InputManager::GetInstance();
	//--

	//-- Level Setup

	char levelFile[64];
	std::snprintf(levelFile, sizeof(levelFile), "Level%d.lvl", levelIndex);

	dae::LevelLoader loader;
	auto tileManagerObj = std::make_shared<dae::GameObject>();
	auto tileManager = tileManagerObj->AddComponent<dae::TileManagerComponent>();

	auto levelManagerObj = std::make_shared<dae::GameObject>();
	auto levelManager = levelManagerObj->AddComponent<dae::LevelManagerComponent>(levelIndex);
	scene.Add(levelManagerObj);

	std::vector<std::vector<std::shared_ptr<dae::GameObject>>> tileGrid;
	loader.LoadLevelBinary(levelFile ,
		scene,
		tileGrid,
		levelManager.get(),
		tileManager.get());

	tileManager->InitWithTileGrid(std::move(tileGrid));

	scene.Add(tileManagerObj);
	//--

	//-- Player Setup
	auto player = std::make_shared<dae::GameObject>();
	int spawnX = 0;
	int spawnY = 0;
	auto spawnPos = loader.GetWorldCenterForTile(spawnX, spawnY);
	spawnPos -= glm::vec3{ 0, 16, 0 };
	player->GetTransform()->SetPosition(spawnPos);

	auto score = player->AddComponent<dae::ScoreComponent>(0);
	auto lives = player->AddComponent<dae::LivesComponent>(3);

	auto render = player->AddComponent<dae::RenderComponent>("CharacterSpriteSheetFilledBackground.png");
	render->SetSize(32, 32);

	player->AddComponent<dae::PlayerDebugComponent>();

	auto animator = player->AddComponent<dae::SpriteAnimatorComponent>(render.get(), 16, 16, 0.2f);

	animator->PlayAnimation(0, 3);

	constexpr int TileWidth = 42;
	constexpr int TileHeight = 43;
	constexpr int OffsetX = (640 - (TileWidth * 15)) / 2; // = 5
	constexpr int OffsetY = 48;

	auto tracker = player->AddComponent<dae::TileTrackerComponent>(TileWidth, TileHeight, OffsetX, OffsetY);
	tracker->SetTrackingMode(dae::TrackingMode::Center);

	auto respawn = player->AddComponent<dae::PlayerRespawnComponent>(spawnPos, levelManager.get());
	lives->AddObserver(respawn);

	player->AddComponent<dae::GemTrackerComponent>();


	scene.Add(player);
	levelManager->RegisterPlayer(player.get());
	//--

	//-- UI Setup
	auto HUDObject = std::make_shared<dae::GameObject>();
	HUDObject->GetTransform()->SetPosition(10, 50, 0);

	auto scoreDisplay = std::make_shared<dae::GameObject>();
	auto scoreText = scoreDisplay->AddComponent<dae::TextComponent>("00000", "ArcadeFont.otf", 24);
	scoreDisplay->GetTransform()->SetPosition(25, 10, 0);
	scoreDisplay->SetParent(HUDObject, false);

	auto hud = HUDObject->AddComponent<dae::HUDDisplay>(scoreText.get(), score.get());

	std::vector<dae::GameObject*> lifeIcons;

	for (int i = 0; i < lives->GetLives(); ++i)
	{
		auto icon = std::make_shared<dae::GameObject>();
		icon->GetTransform()->SetWorldPosition(glm::vec3{ 175.f + i * 45.f, 10.f, 0.f });

		auto renderIcon = icon->AddComponent<dae::RenderComponent>();
		renderIcon->SetTexture("PlayerLifeIcon.png");
		renderIcon->SetSize(32, 32);

		scene.Add(icon);
		lifeIcons.push_back(icon.get());
	}

	hud->SetLifeIcons(lifeIcons);

	score->AddObserver(hud);
	lives->AddObserver(hud);

	scene.Add(HUDObject);
	scene.Add(scoreDisplay);
	//--

	//-- Enemies Setup
	auto spawner = std::make_shared<dae::GameObject>();
	auto spawnerComp = spawner->AddComponent<dae::NobbinSpawnerComponent>(&scene, levelManager.get(), &loader, tileManager.get(),
		14, 0, 5.f, 0);

	lives->AddObserver(spawnerComp);

	scene.Add(spawner);

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

	input.BindCommandKeyboard(SDLK_w, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(player.get(), glm::vec3(0, -1, 0), playerSpeed,
		tileManager.get(), levelManager.get(), animator.get(), dae::AnimationState::WalkUp));
	input.BindCommandKeyboard(SDLK_w, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(animator.get()));

	input.BindCommandKeyboard(SDLK_s, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(player.get(), glm::vec3(0, 1, 0), playerSpeed,
		tileManager.get(), levelManager.get(), animator.get(), dae::AnimationState::WalkDown));
	input.BindCommandKeyboard(SDLK_s, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(animator.get()));

	input.BindCommandKeyboard(SDLK_a, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(player.get(), glm::vec3(-1, 0, 0), playerSpeed,
		tileManager.get(), levelManager.get(), animator.get(), dae::AnimationState::WalkLeft));
	input.BindCommandKeyboard(SDLK_a, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(animator.get()));

	input.BindCommandKeyboard(SDLK_d, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(player.get(), glm::vec3(1, 0, 0), playerSpeed,
		tileManager.get(), levelManager.get(), animator.get(), dae::AnimationState::WalkRight));
	input.BindCommandKeyboard(SDLK_d, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(animator.get()));

	constexpr float kFireballCooldown = 10.f;
	input.BindCommandController(0, dae::GameController::A, dae::InputType::Released,std::make_unique<dae::ShootFireballCommand>(player.get(), 
		tileManager.get(), score.get(), kFireballCooldown));

	input.BindCommandKeyboard(SDLK_SPACE, dae::InputType::Released, std::make_unique<dae::ShootFireballCommand>(player.get(), tileManager.get(),
		score.get(), kFireballCooldown));

	input.BindCommandController(
		0,
		dae::GameController::A,
		dae::InputType::Released,
		std::make_unique<dae::PlaySoundCommand>(dae::ResourceManager::GetInstance().GetFullPath("Explosion Sound Effect.wav"))  // Replace with a valid file
	);
	//--

	//-- Game Logic Setup
	auto resetGO = std::make_shared<dae::GameObject>();
	auto resetComp = resetGO->AddComponent<dae::LevelResetComponent>([](int levelIndex) {
		LoadGame(levelIndex); // your global or static LoadGame
		},
		/* isCoop = */ false);
	lives->AddObserver(resetComp);
	scene.Add(resetGO);
	//--
}

void LoadCoopGame(int levelIndex = 1)
{
	//-- Initial Game Setup
	auto& scene = dae::SceneManager::GetInstance().CreateScene("DiggerCoop");
	const float playerSpeed = 2.f;
	auto& input = dae::InputManager::GetInstance();
	//-- 

	//-- Level Setup
	char levelFile[64];
	std::snprintf(levelFile, sizeof(levelFile), "Level%d.lvl", levelIndex);

	dae::LevelLoader loader;
	auto tileManagerObj = std::make_shared<dae::GameObject>();
	auto tileManager = tileManagerObj->AddComponent<dae::TileManagerComponent>();

	auto levelManagerObj = std::make_shared<dae::GameObject>();
	auto levelManager = levelManagerObj->AddComponent<dae::LevelManagerComponent>(levelIndex);
	scene.Add(levelManagerObj);

	std::vector<std::vector<std::shared_ptr<dae::GameObject>>> tileGrid;
	loader.LoadLevelBinary(levelFile,
		scene,
		tileGrid,
		levelManager.get(),
		tileManager.get());

	tileManager->InitWithTileGrid(std::move(tileGrid));
	scene.Add(tileManagerObj);
	//--

	constexpr int TileWidth = 42;
	constexpr int TileHeight = 43;
	constexpr int OffsetX = (640 - (TileWidth * 15)) / 2;
	constexpr int OffsetY = 48;

	//-- Player 1 Setup 
	auto player1 = std::make_shared<dae::GameObject>();
	glm::vec3 spawnPos1 = loader.GetWorldCenterForTile(0, 0);
	spawnPos1 -= glm::vec3{ 0, 16, 0 };
	player1->GetTransform()->SetPosition(spawnPos1);

	auto score1 = player1->AddComponent<dae::ScoreComponent>(0);
	auto lives1 = player1->AddComponent<dae::LivesComponent>(3);

	auto rend1 = player1->AddComponent<dae::RenderComponent>("CharacterSpriteSheetFilledBackground.png");
	rend1->SetSize(32, 32);

	auto animator1 = player1->AddComponent<dae::SpriteAnimatorComponent>(rend1.get(), 16, 16, 0.2f);
	animator1->PlayAnimation(3, 3);

	// tile tracking as before
	player1->AddComponent<dae::TileTrackerComponent>(
		TileWidth, TileHeight, OffsetX, OffsetY
	)->SetTrackingMode(dae::TrackingMode::Center);

	auto respawn1 = player1->AddComponent<dae::PlayerRespawnComponent>(spawnPos1, levelManager.get());
	lives1->AddObserver(respawn1);

	player1->AddComponent<dae::GemTrackerComponent>();

	scene.Add(player1);
	levelManager->RegisterPlayer(player1.get());

	input.BindCommandController(0, dae::GameController::DPAD_UP, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3(0, -1, 0), playerSpeed,
			tileManager.get(), levelManager.get(), animator1.get(), dae::AnimationState::WalkUp));
	input.BindCommandController(0, dae::GameController::DPAD_UP, dae::InputType::Released,
		std::make_unique<dae::StopAnimationCommand>(animator1.get()));

	input.BindCommandController(0, dae::GameController::DPAD_DOWN, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3(0, 1, 0), playerSpeed,
			tileManager.get(), levelManager.get(), animator1.get(), dae::AnimationState::WalkDown));
	input.BindCommandController(0, dae::GameController::DPAD_DOWN, dae::InputType::Released,
		std::make_unique<dae::StopAnimationCommand>(animator1.get()));

	input.BindCommandController(0, dae::GameController::DPAD_LEFT, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3(-1, 0, 0), playerSpeed,
			tileManager.get(), levelManager.get(), animator1.get(), dae::AnimationState::WalkLeft));
	input.BindCommandController(0, dae::GameController::DPAD_LEFT, dae::InputType::Released,
		std::make_unique<dae::StopAnimationCommand>(animator1.get()));

	input.BindCommandController(0, dae::GameController::DPAD_RIGHT, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(player1.get(), glm::vec3(1, 0, 0), playerSpeed,
			tileManager.get(), levelManager.get(), animator1.get(), dae::AnimationState::WalkRight));
	input.BindCommandController(0, dae::GameController::DPAD_RIGHT, dae::InputType::Released,
		std::make_unique<dae::StopAnimationCommand>(animator1.get()));

	constexpr float kFireballCooldown = 10.f;
	input.BindCommandController(0, dae::GameController::A, dae::InputType::Released,
		std::make_unique<dae::ShootFireballCommandIndividual>(player1.get(),
			tileManager.get(), score1.get(), kFireballCooldown));
	//--

	//-- Player 2 Setup 
	auto player2 = std::make_shared<dae::GameObject>();
	glm::vec3 spawnPos2 = loader.GetWorldCenterForTile(7, 9);
	spawnPos2 -= glm::vec3{ 0, 16, 0 };
	player2->GetTransform()->SetPosition(spawnPos2);

	auto score2 = player2->AddComponent<dae::ScoreComponent>(0);
	auto lives2 = player2->AddComponent<dae::LivesComponent>(3);

	auto rend2 = player2->AddComponent<dae::RenderComponent>("CharacterSpriteSheetFilledBackground.png");
	rend2->SetSize(32, 32);

	auto animator2 = player2->AddComponent<dae::SpriteAnimatorComponent>(rend2.get(), 16, 16, 0.2f);
	animator2->PlayAnimation(3, 3);

	player2->AddComponent<dae::TileTrackerComponent>(
		TileWidth, TileHeight, OffsetX, OffsetY
	)->SetTrackingMode(dae::TrackingMode::Center);

	auto respawn2 = player2->AddComponent<dae::PlayerRespawnComponent>(spawnPos2, levelManager.get());
	lives2->AddObserver(respawn2);

	player2->AddComponent<dae::GemTrackerComponent>();

	scene.Add(player2);
	levelManager->RegisterPlayer(player2.get());

	input.BindCommandKeyboard(SDLK_w, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3(0, -1, 0), playerSpeed,
		tileManager.get(), levelManager.get(), animator2.get(), dae::AnimationState::WalkUp));
	input.BindCommandKeyboard(SDLK_w, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(animator2.get()));

	input.BindCommandKeyboard(SDLK_s, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3(0, 1, 0), playerSpeed,
		tileManager.get(), levelManager.get(), animator2.get(), dae::AnimationState::WalkDown));
	input.BindCommandKeyboard(SDLK_s, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(animator2.get()));

	input.BindCommandKeyboard(SDLK_a, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3(-1, 0, 0), playerSpeed,
		tileManager.get(), levelManager.get(), animator2.get(), dae::AnimationState::WalkLeft));
	input.BindCommandKeyboard(SDLK_a, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(animator2.get()));

	input.BindCommandKeyboard(SDLK_d, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(player2.get(), glm::vec3(1, 0, 0), playerSpeed,
		tileManager.get(), levelManager.get(), animator2.get(), dae::AnimationState::WalkRight));
	input.BindCommandKeyboard(SDLK_d, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(animator2.get()));

	input.BindCommandKeyboard(SDLK_SPACE, dae::InputType::Released,
		std::make_unique<dae::ShootFireballCommandIndividual>(player2.get(),
			tileManager.get(), score2.get(), kFireballCooldown));
	//--

	//-- HUD 1
	auto hudObj1 = std::make_shared<dae::GameObject>();
	hudObj1->GetTransform()->SetPosition(10, 50, 0);
	auto scoreDisplay1 = std::make_shared<dae::GameObject>();
	auto scoreText1 = scoreDisplay1->AddComponent<dae::TextComponent>("00000", "ArcadeFont.otf", 24);
	scoreDisplay1->GetTransform()->SetPosition(25, 10, 0);
	scoreDisplay1->SetParent(hudObj1, false);
	auto hud1 = hudObj1->AddComponent<dae::HUDDisplay>(scoreText1.get(), score1.get());

	std::vector<dae::GameObject*> lifeIcons1;

	for (int i = 0; i < lives1->GetLives(); ++i)
	{
		auto icon = std::make_shared<dae::GameObject>();
		icon->GetTransform()->SetWorldPosition(glm::vec3{ 175.f + i * 45.f, 10.f, 0.f });

		auto renderIcon = icon->AddComponent<dae::RenderComponent>();
		renderIcon->SetTexture("PlayerLifeIcon.png");
		renderIcon->SetSize(32, 32);

		scene.Add(icon);
		lifeIcons1.push_back(icon.get());
	}

	hud1->SetLifeIcons(lifeIcons1);

	scene.Add(hudObj1); scene.Add(scoreDisplay1);
	score1->AddObserver(hud1);
	lives1->AddObserver(hud1);

	//-- HUD 2 (shifted right by e.g. 300px)
	auto hudObj2 = std::make_shared<dae::GameObject>();
	hudObj2->GetTransform()->SetPosition(300, 50, 0);
	auto scoreDisplay2 = std::make_shared<dae::GameObject>();
	auto scoreText2 = scoreDisplay2->AddComponent<dae::TextComponent>("00000", "ArcadeFont.otf", 24);
	scoreDisplay2->GetTransform()->SetPosition(325, 10, 0);
	scoreDisplay2->SetParent(hudObj2, false);
	auto hud2 = hudObj2->AddComponent<dae::HUDDisplay>(scoreText2.get(), score2.get());

	std::vector<dae::GameObject*> lifeIcons2;

	for (int i = 0; i < lives2->GetLives(); ++i)
	{
		auto icon = std::make_shared<dae::GameObject>();
		icon->GetTransform()->SetWorldPosition(glm::vec3{ 475.f + i * 45.f, 10.f, 0.f });

		auto renderIcon = icon->AddComponent<dae::RenderComponent>();
		renderIcon->SetTexture("PlayerLifeIcon.png");
		renderIcon->SetSize(32, 32);

		scene.Add(icon);
		lifeIcons2.push_back(icon.get());
	}

	hud2->SetLifeIcons(lifeIcons2);

	scene.Add(hudObj2); scene.Add(scoreDisplay2);
	score2->AddObserver(hud2);
	lives2->AddObserver(hud2);
	//--

	//-- Enemies Setup
	auto spawner = std::make_shared<dae::GameObject>();
	auto spawnerComp = spawner->AddComponent<dae::NobbinSpawnerComponent>(
		&scene, levelManager.get(), &loader, tileManager.get(), 14, 0, 5.f, 0);

	lives1->AddObserver(spawnerComp);
	lives2->AddObserver(spawnerComp);

	scene.Add(spawner);

	//--

	//-- Game Logic Setup
	auto resetGO = std::make_shared<dae::GameObject>();
	auto resetComp = resetGO->AddComponent<dae::LevelResetComponent>([](int levelIndex) {
		LoadCoopGame(levelIndex); // your global or static LoadGame
		},
		/* isCoop = */ false);
	lives1->AddObserver(resetComp);
	lives2->AddObserver(resetComp);
	scene.Add(resetGO);
	//--
}

void LoadVersusGame(int levelIndex = 1)
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("DiggerVersus");
	const float playerSpeed = 2.f;
	auto& input = dae::InputManager::GetInstance();

	//-- Level setup
	char levelFile[64];
	std::snprintf(levelFile, sizeof(levelFile), "Level%d.lvl", levelIndex);

	dae::LevelLoader loader;
	auto tileManagerObj = std::make_shared<dae::GameObject>();
	auto tileManager = tileManagerObj->AddComponent<dae::TileManagerComponent>();
	scene.Add(tileManagerObj);

	auto levelManagerObj = std::make_shared<dae::GameObject>();
	auto levelManager = levelManagerObj->AddComponent<dae::LevelManagerComponent>(levelIndex);
	scene.Add(levelManagerObj);

	std::vector<std::vector<std::shared_ptr<dae::GameObject>>> tileGrid;
	loader.LoadLevelBinary(levelFile, scene, tileGrid, levelManager.get(), tileManager.get());
	tileManager->InitWithTileGrid(std::move(tileGrid));
	//--

	//-- Player Setup
	auto player = std::make_shared<dae::GameObject>();
	int spawnX = 0;
	int spawnY = 0;
	auto spawnPos = loader.GetWorldCenterForTile(spawnX, spawnY);
	spawnPos -= glm::vec3{ 0, 16, 0 };
	player->GetTransform()->SetPosition(spawnPos);

	auto score = player->AddComponent<dae::ScoreComponent>(0);
	auto lives = player->AddComponent<dae::LivesComponent>(3);

	auto render = player->AddComponent<dae::RenderComponent>("CharacterSpriteSheetFilledBackground.png");
	render->SetSize(32, 32);

	player->AddComponent<dae::PlayerDebugComponent>();

	auto animator = player->AddComponent<dae::SpriteAnimatorComponent>(render.get(), 16, 16, 0.2f);

	animator->PlayAnimation(3, 3);

	constexpr int TileWidth = 42;
	constexpr int TileHeight = 43;
	constexpr int OffsetX = (640 - (TileWidth * 15)) / 2; // = 5
	constexpr int OffsetY = 48;

	auto tracker = player->AddComponent<dae::TileTrackerComponent>(TileWidth, TileHeight, OffsetX, OffsetY);
	tracker->SetTrackingMode(dae::TrackingMode::Center);

	auto respawn = player->AddComponent<dae::PlayerRespawnComponent>(spawnPos, levelManager.get());
	lives->AddObserver(respawn);

	player->AddComponent<dae::GemTrackerComponent>();

	scene.Add(player);
	levelManager->RegisterPlayer(player.get());
	//-- Player Controls Setup
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

	constexpr float kFireballCooldown = 10.f;
	input.BindCommandController(0, dae::GameController::A, dae::InputType::Released,
		std::make_unique<dae::ShootFireballCommandIndividual>(player.get(),
			tileManager.get(), score.get(), kFireballCooldown));
	//--

	//-- Hud Player
	//-- HUD 1
	auto hudObj1 = std::make_shared<dae::GameObject>();
	hudObj1->GetTransform()->SetPosition(10, 50, 0);
	auto scoreDisplay1 = std::make_shared<dae::GameObject>();
	auto scoreText1 = scoreDisplay1->AddComponent<dae::TextComponent>("00000", "ArcadeFont.otf", 24);
	scoreDisplay1->GetTransform()->SetPosition(25, 10, 0);
	scoreDisplay1->SetParent(hudObj1, false);
	auto hud1 = hudObj1->AddComponent<dae::HUDDisplay>(scoreText1.get(), score.get());

	std::vector<dae::GameObject*> lifeIcons1;

	for (int i = 0; i < lives->GetLives(); ++i)
	{
		auto icon = std::make_shared<dae::GameObject>();
		icon->GetTransform()->SetWorldPosition(glm::vec3{ 175.f + i * 45.f, 10.f, 0.f });

		auto renderIcon = icon->AddComponent<dae::RenderComponent>();
		renderIcon->SetTexture("PlayerLifeIcon.png");
		renderIcon->SetSize(32, 32);

		scene.Add(icon);
		lifeIcons1.push_back(icon.get());
	}

	hud1->SetLifeIcons(lifeIcons1);

	scene.Add(hudObj1); scene.Add(scoreDisplay1);
	score->AddObserver(hud1);
	lives->AddObserver(hud1);
	//--

	//-- Nobbbin Player Setup
	auto nobbinPlayer = std::make_shared<dae::GameObject>();
	int nPSpawnX = 14;
	int nPSpawnY = 0;
	glm::vec3 nPSpawnPos = loader.GetWorldCenterForTile(nPSpawnX, nPSpawnY);
	nPSpawnPos -= glm::vec3{ 0, 16, 0 };
	nobbinPlayer->GetTransform()->SetPosition(nPSpawnPos);

	auto nPRender = nobbinPlayer->AddComponent<dae::RenderComponent>("NormalNobbinSpritesheet.png");
	nPRender->SetSize(32, 32);

	nobbinPlayer->AddComponent<dae::PlayerDebugComponent>();

	auto nPAnimator = nobbinPlayer->AddComponent<dae::SpriteAnimatorComponent>(nPRender.get(), 16, 16, 0.2f);

	nPAnimator->PlayAnimation(3, 3);

	auto nPTracker = nobbinPlayer->AddComponent<dae::TileTrackerComponent>(TileWidth, TileHeight, OffsetX, OffsetY);
	nPTracker->SetTrackingMode(dae::TrackingMode::Center);

	auto nPRespawn = nobbinPlayer->AddComponent<dae::PlayerRespawnComponent>(nPSpawnPos, levelManager.get());

	nobbinPlayer->AddComponent<dae::VersusDamageComponent>(levelManager.get(), nPSpawnPos, 5.f);

	nobbinPlayer->AddComponent<dae::DigUnlockComponent>(60.f, 15.f);

	scene.Add(nobbinPlayer);
	//levelManager->RegisterPlayer(player.get());
	 
	input.BindCommandKeyboard(SDLK_w, dae::InputType::Pressed, std::make_unique<dae::MoveCommandNobbin>(nobbinPlayer.get(), glm::vec3(0, -1, 0), playerSpeed,
		tileManager.get(), levelManager.get(), nPAnimator.get(), dae::AnimationState::WalkUp));
	input.BindCommandKeyboard(SDLK_w, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(nPAnimator.get()));

	input.BindCommandKeyboard(SDLK_s, dae::InputType::Pressed, std::make_unique<dae::MoveCommandNobbin>(nobbinPlayer.get(), glm::vec3(0, 1, 0), playerSpeed,
		tileManager.get(), levelManager.get(), nPAnimator.get(), dae::AnimationState::WalkDown));
	input.BindCommandKeyboard(SDLK_s, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(nPAnimator.get()));

	input.BindCommandKeyboard(SDLK_a, dae::InputType::Pressed, std::make_unique<dae::MoveCommandNobbin>(nobbinPlayer.get(), glm::vec3(-1, 0, 0), playerSpeed,
		tileManager.get(), levelManager.get(), nPAnimator.get(), dae::AnimationState::WalkLeft));
	input.BindCommandKeyboard(SDLK_a, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(nPAnimator.get()));

	input.BindCommandKeyboard(SDLK_d, dae::InputType::Pressed, std::make_unique<dae::MoveCommandNobbin>(nobbinPlayer.get(), glm::vec3(1, 0, 0), playerSpeed,
		tileManager.get(), levelManager.get(), nPAnimator.get(), dae::AnimationState::WalkRight));
	input.BindCommandKeyboard(SDLK_d, dae::InputType::Released, std::make_unique<dae::StopAnimationCommand>(nPAnimator.get()));
	//--

	//-- Other Nobbin spawn
	auto spawner = std::make_shared<dae::GameObject>();
	auto spawnerComp = spawner->AddComponent<dae::NobbinSpawnerComponent>(
		&scene, levelManager.get(), &loader, tileManager.get(), 14, 0, 5.f, 0);

	lives->AddObserver(spawnerComp);

	scene.Add(spawner);
	//--

	//-- Game Logic Setup
	auto resetGO = std::make_shared<dae::GameObject>();
	auto resetComp = resetGO->AddComponent<dae::LevelResetComponent>([](int levelIndex) {
		LoadVersusGame(levelIndex); // your global or static LoadGame
		},
		/* isCoop = */ false);
	lives->AddObserver(resetComp);
	scene.Add(resetGO);
	//--
}

int main(int, char* [])
{
	dae::Minigin engine("../Data/");
	dae::ResourceManager::GetInstance().Init("../Data");

	auto soundSystem = new dae::SDLSoundSystem();

	dae::SoundServiceLocator::Provide(soundSystem);
	//engine.Run(load);
	engine.Run([] {
		LoadVersusGame(1); // start with Level 1
		});
	return 0;
}

