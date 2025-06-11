#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include <functional>

namespace dae
{
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		Scene& CreateScene(const std::string& name);

		void Update(float deltaTime);
		void Render();

		Scene* GetCurrentScene();

		void ClearScenes();

		void RequestReload(std::function<void()> reloadCallback);
		bool HasPendingReload() const;
		void PerformReload();  // call this outside of Update()
	private:
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::shared_ptr<Scene>> m_scenes;

		bool m_PendingReload = false;
		std::function<void()> m_ReloadCallback;
	};
}
