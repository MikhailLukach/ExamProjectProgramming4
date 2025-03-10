#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	dae::ResourceManager::GetInstance().Init("../Data");

	auto& input = dae::InputManager::GetInstance();

	const float char1Speed{ 5.f };
	const float char2Speed{ 10.f };

	/*input.BindCommandKeyboard(SDLK_w, dae::InputType::Pressed, std::make_unique<dae::JumpCommand>());
	input.BindCommandKeyboard(SDLK_SPACE, dae::InputType::Down, std::make_unique<dae::FireCommand>());

	input.BindCommandController(0, XINPUT_GAMEPAD_DPAD_UP, dae::InputType::Down, std::make_unique<dae::JumpCommand>());
	input.BindCommandController(0, XINPUT_GAMEPAD_DPAD_DOWN, dae::InputType::Pressed, std::make_unique<dae::FireCommand>());*/



	std::cout << "input commands have been set\n";

	auto go = std::make_shared<dae::GameObject>();
	go->GetTransform()->SetPosition(0, 0, 0);
	go->AddComponent<dae::RenderComponent>("background.tga");
	scene.Add(go);

	auto go2 = std::make_shared<dae::GameObject>();
	go2->GetTransform()->SetPosition(225, 100, 0);
	go2->AddComponent<dae::RenderComponent>("logo.tga");
	scene.Add(go2);

	auto go3 = std::make_shared<dae::GameObject>();
	go3->GetTransform()->SetPosition(175, 50, 0);
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
	scene.Add(go5);

	auto go6 = std::make_shared<dae::GameObject>();
	go6->GetTransform()->SetPosition(300, 350, 0);
	go6->AddComponent<dae::RenderComponent>("MrDo!Enemy.png");
	scene.Add(go6);

	//input for gameobject go5 and go6
	input.BindCommandKeyboard(SDLK_w, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(go5.get(), glm::vec3(0, -1, 0), char1Speed));
	input.BindCommandKeyboard(SDLK_s, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(go5.get(), glm::vec3(0, 1, 0), char1Speed));
	input.BindCommandKeyboard(SDLK_a, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(go5.get(), glm::vec3(-1, 0, 0), char1Speed));
	input.BindCommandKeyboard(SDLK_d, dae::InputType::Pressed, std::make_unique<dae::MoveCommand>(go5.get(), glm::vec3(1, 0, 0), char1Speed));

	input.BindCommandController(0, dae::GameController::DPAD_UP, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(go6.get(), glm::vec3(0, -1, 0), char2Speed));
	input.BindCommandController(0, dae::GameController::DPAD_DOWN, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(go6.get(), glm::vec3(0, 1, 0), char2Speed));
	input.BindCommandController(0, dae::GameController::DPAD_LEFT, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(go6.get(), glm::vec3(-1, 0, 0), char2Speed));
	input.BindCommandController(0, dae::GameController::DPAD_RIGHT, dae::InputType::Pressed,
		std::make_unique<dae::MoveCommand>(go6.get(), glm::vec3(1, 0, 0), char2Speed));

	//auto go5 = std::make_shared<dae::GameObject>();
	//go5->AddComponent<dae::CacheTestComponent>();
	//scene.Add(go5);

	//parent and child
	/*auto mainGameObject = std::make_shared<dae::GameObject>();
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
	scene.Add(childGameObject);*/

	//testing stuff
	/*if (go->GetComponent<dae::RenderComponent>()->GetOwner() == go.get())
	{
		std::cout << "GetOwner() correctly returns the GameObject!\n";
	}
	else
	{
		std::cerr << "Error: GetOwner() does not match the original GameObject!\n";
	}*/

	//std::cout << "X:" << childGameObject->GetComponent<dae::Transform>()->GetPosition().x << std::endl;
	//std::cout << "Y:" << childGameObject->GetComponent<dae::Transform>()->GetPosition().y << std::endl;
	/*if (go->HasComponent<dae::Transform>())
	{
		std::cout << "Transform component successfully added!\n";

		auto retrievedTransform = go->GetComponent<dae::Transform>();
		glm::vec3 position = retrievedTransform->GetPosition();

		std::cout << "Position: (" << position.x << ", " << position.y << ", " << position.z << ")\n";
	}
	else
	{
		std::cout << "Error: Transform component not found!\n";
	}*/
}

int main(int, char*[]) {
	dae::Minigin engine("../Data/");
	engine.Run(load);
    return 0;
}