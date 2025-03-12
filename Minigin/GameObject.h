#pragma once
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <iostream>
#include <vector>
#include "Transform.h"
#include "RenderComponent.h"
#include "TextComponent.h"
#include "FPSComponent.h"
#include "RotatorComponent.h"
#include "CacheTestComponent.h"
#include "HealthComponent.h"
#include "Component.h"

namespace dae
{
	class Component;
	class GameObject final : public std::enable_shared_from_this<GameObject>
	{
	public:
		void Update(float deltaTime);
		void Render() const;

		GameObject();
		~GameObject();

		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		std::shared_ptr<GameObject> GetParent() const;
		void SetParent(std::shared_ptr<GameObject> newParent, bool keepWorldPosition = true);
		bool IsChild(std::shared_ptr<const GameObject> potentialParent);
		size_t GetChildCount() const { return m_pVecChildren.size(); }
		std::shared_ptr<GameObject> GetChildAt(size_t index) const;

		void RemoveChild(std::shared_ptr<GameObject> child);

		void MarkForDeletion() { m_MarkedForDeletion = true; }
		bool IsMarkedForDeletion() const { return m_MarkedForDeletion; }

		//void SetLocalPosition(const glm::vec3& pos);
		//glm::vec3 GetWorldPosition();
		//void UpdateWorldTransform();

		std::shared_ptr<Transform> GetTransform() const { return m_pTransform; }

		template <typename T, typename... Args>
		std::shared_ptr<T> AddComponent(Args&&... args);

		template <typename T>
		void RemoveComponent();

		template <typename T>
		std::shared_ptr<T> GetComponent();

		template <typename T>
		bool HasComponent() const;
	private:
		std::weak_ptr<GameObject> m_pParent;
		std::vector<std::shared_ptr<GameObject>> m_pVecChildren;
		std::unordered_map<std::type_index, std::shared_ptr<Component>> m_components;
		std::shared_ptr<Transform> m_pTransform;

		glm::vec3 m_localPosition{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_worldPosition{ 0.0f, 0.0f, 0.0f };
		float m_rotation = 0.0f;
		glm::vec3 m_scale{ 1.0f, 1.0f, 1.0f };

		bool m_transformDirty = true;
		bool m_MarkedForDeletion{ false };

		void MarkTransformDirty();
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

		auto it = m_components.find(typeIndex);
		if (it != m_components.end())
		{
			it->second.reset();
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
