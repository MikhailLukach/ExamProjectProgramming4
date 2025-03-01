#pragma once
#include <memory>
#include <iostream>

namespace dae
{
	class GameObject;

	class Component
	{
	public:
		virtual ~Component() = default;
		virtual void Update(float deltaTime) { (void)deltaTime; } 
		virtual void Render() const {}

		void SetOwner(GameObject* owner)
		{ 
			if (m_owner == nullptr)
			{
				m_owner = owner;
			}
			else
			{
				std::cerr << "Warning: You are attempting to change component owner after assignment" << std::endl;
			}
		}
		GameObject* GetOwner() const { return m_owner; }
	protected:
		GameObject* m_owner = nullptr;
	};
}