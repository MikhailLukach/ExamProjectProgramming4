#pragma once
#include <iostream>
#include "GameObject.h"
namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};

    class JumpCommand : public Command
    {
    public:
        void Execute() override
        {
            std::cout << "Jump!\n";
        }
    };

    class FireCommand : public Command
    {
    public:
        void Execute() override
        {
            std::cout << "Fire!\n";
        }
    };

	class MoveCommand : public Command
	{
	public:
		MoveCommand(GameObject* character, const glm::vec3& direction, float speed)
			: m_Character(character), m_Direction(direction), m_Speed(speed) 
		{

		}

		void Execute() override
		{
			if (m_Character)
			{
				auto transform = m_Character->GetTransform();
				glm::vec3 newPosition = transform->GetWorldPosition() + glm::normalize(m_Direction) * m_Speed;
				transform->SetPosition(newPosition);
			}
		}

	private:
		GameObject* m_Character;
		glm::vec3 m_Direction;
		float m_Speed;
	};
}

