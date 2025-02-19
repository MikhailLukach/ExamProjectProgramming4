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

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");
	dae::ResourceManager::GetInstance().Init("../Data");

	auto go = std::make_shared<dae::GameObject>();
	go->AddComponent<dae::Transform>()->SetPosition(0,0,0);
	go->AddComponent<dae::RenderComponent>(go->GetComponent<dae::Transform>(), "background.tga");
	scene.Add(go);

	auto go2 = std::make_shared<dae::GameObject>();
	go2->AddComponent<dae::Transform>()->SetPosition(225, 100, 0);
	go2->AddComponent<dae::RenderComponent>(go2->GetComponent<dae::Transform>(), "logo.tga");
	scene.Add(go2);

	auto go3 = std::make_shared<dae::GameObject>();
	go3->AddComponent<dae::Transform>()->SetPosition(175, 50, 0);
	go3->AddComponent<dae::TextComponent>(go3->GetComponent<dae::Transform>(), "Programming 4 Assignment", "Lingua.otf", 24);
	scene.Add(go3);

	auto go4 = std::make_shared<dae::GameObject>();
	go4->AddComponent<dae::Transform>()->SetPosition(0, 0, 0);
	go4->AddComponent<dae::TextComponent>(go4->GetComponent<dae::Transform>(), "FPS: 0", "Lingua.otf", 24);
	go4->AddComponent<dae::FPSComponent>(go4->GetComponent<dae::TextComponent>());
	scene.Add(go4);

	auto mainGameObject = std::make_shared<dae::GameObject>();
	mainGameObject->AddComponent<dae::Transform>()->SetPosition(300, 300, 0);
	mainGameObject->AddComponent<dae::RenderComponent>(mainGameObject->GetComponent<dae::Transform>()
		, "Mr!DoClown.png");
	mainGameObject->AddComponent<dae::RotatorComponent>(mainGameObject->GetComponent<dae::Transform>()
		, 50.0f, 90.0f, glm::vec3(300, 300, 0));

	auto childGameObject = std::make_shared<dae::GameObject>();
	childGameObject->AddComponent<dae::Transform>()->SetPosition(300, 350, 0);
	childGameObject->AddComponent<dae::RenderComponent>(childGameObject->GetComponent<dae::Transform>()
		, "MrDo!Enemy.png");
	childGameObject->AddComponent<dae::RotatorComponent>(childGameObject->GetComponent<dae::Transform>()
		, 50.0f, 90.0f, glm::vec3(350, 350, 0));

	childGameObject->SetParent(mainGameObject, true);

	if (childGameObject->GetParent() == mainGameObject)
	{
		std::cout << "Orbiting Character is correctly parented.\n";
	}

	scene.Add(mainGameObject);
	scene.Add(childGameObject);

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