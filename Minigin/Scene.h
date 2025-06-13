#pragma once
#include "SceneManager.h"
#include "GameObject.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(std::shared_ptr<GameObject> object);
		void RemoveAll();

		void Update(float deltaTime);
		void Render() const;

		void RemoveDeletedObjects();

		template <typename T>
		std::shared_ptr<GameObject> FindObjectWithComponent() const;

		template <typename T>
		std::vector<std::shared_ptr<GameObject>> FindObjectsWithComponent() const;

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		const std::vector<std::shared_ptr<GameObject>>& GetObjects() const;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector < std::shared_ptr<GameObject>> m_objects{};

		static unsigned int m_idCounter; 
	};

	template<typename T>
	inline std::shared_ptr<GameObject> Scene::FindObjectWithComponent() const
	{
		for (const auto& obj : m_objects)
		{
			if (obj && obj->GetComponent<T>() != nullptr)
				return obj;
		}
		return nullptr;
	}

	template<typename T>
	inline std::vector<std::shared_ptr<GameObject>> Scene::FindObjectsWithComponent() const
	{
		std::vector<std::shared_ptr<dae::GameObject>> results;
		for (const auto& obj : m_objects)
		{
			if (obj && obj->GetComponent<T>() != nullptr)
				results.push_back(obj);
		}
		return results;
	}

}
