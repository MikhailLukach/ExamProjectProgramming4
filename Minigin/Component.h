#pragma once
#include <memory>
#include <iostream>
//#include "GameObject.h"

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;
		virtual void Update([[maybe_unused]] float deltaTime) {}
		virtual void Render() const {}

		void SetOwner(GameObject* owner) 
		{ 
			m_owner = owner;
			std::cout << "SetOwner called for component, owner: " << m_owner << std::endl;
		}
		GameObject* GetOwner() const { return m_owner; }
	protected:
		GameObject* m_owner = nullptr;
	};
}