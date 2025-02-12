#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

dae::GameObject::~GameObject() = default;

void dae::GameObject::Update([[maybe_unused]] float deltaTime)
{
	for (auto& [type, component] : m_components)
	{
		component->Update(deltaTime);
	}
}

void dae::GameObject::Render() const
{
	//const auto& pos = m_transform.GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	for (const auto& [type, component] : m_components)
	{
		component->Render();
	}
}

/*void dae::GameObject::SetTexture([[maybe_unused]] const std::string& filename)
{
	//m_texture = ResourceManager::GetInstance().LoadTexture(filename);
}

void dae::GameObject::SetPosition([[maybe_unused]] float x, [[maybe_unused]] float y)
{
	//m_transform.SetPosition(x, y, 0.0f);
}*/
