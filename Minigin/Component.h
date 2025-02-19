#pragma once
#include <memory>
#include <iostream>
//#include "GameObject.h"

namespace dae
{
	class GameObject;

	class Component
	{
		//GameObject* m_owner = nullptr;
	public:
		virtual ~Component() = default;
		virtual void Update([[maybe_unused]] float deltaTime) { } //fix this later, ok?
		virtual void Render() const {}

		//just remove set owner actually, if good enough for unity, good enough for me
		//pass reference instead of pointer
		void SetOwner(GameObject* owner)
		{ 
			//tell previous owner that he no longer controls this component
			//tell the new owner that he is the new owner
			//check whether the gameobject is a valid new owner
			m_owner = owner;
		}
		GameObject* GetOwner() const { return m_owner; }
	protected:
		GameObject* m_owner = nullptr;
	};
}