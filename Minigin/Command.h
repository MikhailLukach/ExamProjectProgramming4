#pragma once
#include <iostream>
#include "GameObject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"

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

    class KillPlayerCommand : public Command
    {
    public:
        explicit KillPlayerCommand(HealthComponent* health)
            : m_Health(health) {}

        void Execute() override
        {
            if (m_Health)
            {
                m_Health->TakeDamage(1);
                std::cout << "[DEBUG] KillPlayerCommand Executed!\n";
            }
        }

    private:
        HealthComponent* m_Health;
    };

    class AddScoreCommand : public Command
    {
    public:
        explicit AddScoreCommand(ScoreComponent* score, int points)
            : m_pScoreComponent(score),m_Points(points) {}

        void Execute() override
        {
            if (m_pScoreComponent)
            {
                m_pScoreComponent->AddPoints(m_Points);
                std::cout << "[DEBUG] AddScoreCommand Executed!\n";
            }
        }

    private:
        ScoreComponent* m_pScoreComponent;
        int m_Points;
    };
}

