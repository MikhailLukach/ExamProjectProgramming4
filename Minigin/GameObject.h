#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <iostream>
#include "Transform.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "Component.h"

namespace dae
{
	//class Texture2D;
	class Component;
	// todo: this should become final. Done
	class GameObject final
	{
	public:
		virtual void Update(float deltaTime);
		virtual void Render() const;

		//void SetTexture(const std::string& filename);
		//void SetPosition(float x, float y);

		GameObject() = default;
		virtual ~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		template <typename T, typename... Args>
		std::shared_ptr<T> AddComponent(Args&&... args);

		template <typename T>
		void RemoveComponent();

		template <typename T>
		std::shared_ptr<T> GetComponent();

		template <typename T>
		bool HasComponent() const;


	private:
		//Transform m_transform{};
		// todo: mmm, every gameobject has a texture? Is that correct?
		//std::shared_ptr<Texture2D> m_texture{};
		std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
	};

	template<typename T, typename... Args>
	std::shared_ptr<T> GameObject::AddComponent(Args && ...args)
	{
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
		std::type_index typeIndex(typeid(T));

		if (m_components.find(typeIndex) != m_components.end())
		{
			std::cerr << "Warning: Component already exists!" << std::endl;
			return std::static_pointer_cast<T>(m_components[typeIndex]);
		}
		auto component = std::make_shared<T>(std::forward<Args>(args)...);
		component->SetOwner(this);
		m_components[typeIndex] = component;

		return component;
	}

	template<typename T>
	inline void GameObject::RemoveComponent()
	{
		std::type_index typeIndex(typeid(T));

		if (m_components.find(typeIndex) != m_components.end())
		{
			m_components.erase(typeIndex);
		}
	}

	template<typename T>
	std::shared_ptr<T> GameObject::GetComponent()
	{
		std::type_index typeIndex(typeid(T));

		if (m_components.find(typeIndex) != m_components.end())
		{
			return std::static_pointer_cast<T>(m_components[typeIndex]);
		}
		
		std::cerr << "Warning: Unable to get component!" << std::endl;
		return nullptr;
	}

	template<typename T>
	bool GameObject::HasComponent() const
	{
		return m_components.find(typeid(T)) != m_components.end();
	}


}
