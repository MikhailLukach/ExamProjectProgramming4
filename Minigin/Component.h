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
		virtual void Update([[maybe_unused]] float deltaTime) { } //fix this later, ok?
		virtual void Render() const {}

		void SetOwner(GameObject* owner)
		{ 
			m_owner = owner;
		}
		GameObject* GetOwner() const { return m_owner; }
	protected:
		GameObject* m_owner = nullptr;
	};
}