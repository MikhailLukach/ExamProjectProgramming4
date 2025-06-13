#include "Scene.h"

#include <algorithm>

using namespace dae;

unsigned int Scene::m_idCounter = 0;

const std::vector<std::shared_ptr<dae::GameObject>>& dae::Scene::GetObjects() const
{
	return m_objects;
}

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::shared_ptr<GameObject> object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(std::shared_ptr<GameObject> object)
{
	//m_objects.erase(std::remove(m_objects.begin(), m_objects.end(), object), m_objects.end());
	object->MarkForDeletion();
}

void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}

	RemoveDeletedObjects();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

void dae::Scene::RemoveDeletedObjects()
{
	m_objects.erase(
		std::remove_if(m_objects.begin(), m_objects.end(),
			[](const std::shared_ptr<GameObject>& obj) {
				return obj->IsMarkedForDeletion();
			}),
		m_objects.end());
}

