#include "SceneManager.h"
#include "Scene.h"

void dae::SceneManager::Update(float deltaTime)
{
	for(auto& scene : m_scenes)
	{
		scene->Update(deltaTime);
	}

	for (auto& scene : m_scenes)
	{
		scene->RemoveDeletedObjects();
	}
}

void dae::SceneManager::Render()
{
	for (const auto& scene : m_scenes)
	{
		scene->Render();
	}
}

dae::Scene* dae::SceneManager::GetCurrentScene()
{
	if (!m_scenes.empty())
		return m_scenes.back().get(); 
	return nullptr;
}

void dae::SceneManager::ClearScenes()
{
	m_scenes.clear();
}

void dae::SceneManager::RequestReload(std::function<void()> reloadCallback)
{
	m_PendingReload = true;
	m_ReloadCallback = std::move(reloadCallback);
}

bool dae::SceneManager::HasPendingReload() const
{
	return m_PendingReload;
}

void dae::SceneManager::PerformReload()
{
	if (!m_PendingReload) return;
	ClearScenes();
	// Invoke your game’s LoadGame (or equivalent) function
	m_ReloadCallback();
	m_PendingReload = false;
}

dae::Scene& dae::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_scenes.push_back(scene);
	return *scene;
}
