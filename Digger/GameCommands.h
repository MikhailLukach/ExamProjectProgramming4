#pragma once
#include <iostream>
#include "GameObject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "SpriteAnimatorComponent.h"
#include "AnimationState.h"


namespace dae
{
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
        MoveCommand(GameObject* character, const glm::vec3& direction, float speed,
            SpriteAnimatorComponent* animator = nullptr, AnimationState state = AnimationState::WalkDown)
            : m_Character(character), m_Direction(direction), m_Speed(speed),
            m_pAnimator(animator), m_AnimState(state) {}

        void Execute() override
        {
            if (m_Character)
            {
                glm::vec3 clampedDirection = m_Direction;

                if (std::abs(m_Direction.x) > std::abs(m_Direction.y))
                {
                    clampedDirection = { m_Direction.x > 0 ? 1.0f : -1.0f, 0.0f, 0.0f };
                }
                else if (std::abs(m_Direction.y) > 0.0f)
                {
                    clampedDirection = { 0.0f, m_Direction.y > 0 ? 1.0f : -1.0f, 0.0f };
                }
                else
                {
                    clampedDirection = { 0.0f, 0.0f, 0.0f }; // No input
                }

                auto transform = m_Character->GetTransform();
                glm::vec3 newPosition = transform->GetWorldPosition() + glm::normalize(m_Direction) * m_Speed;
                std::cout << "Clamped Dir: (" << clampedDirection.x << ", " << clampedDirection.y << ")\n";
                transform->SetPosition(newPosition);

                // Play animation if animator is set
                if (m_pAnimator)
                {
                    switch (m_AnimState)
                    {
                    case AnimationState::WalkRight: 
                        m_pAnimator->PlayAnimation(0, 3); 
                        //std::cout << "Execute Walking right" << std::endl;
                        break;
                    case AnimationState::WalkDown: 
                        m_pAnimator->PlayAnimation(3, 3); 
                        break;
                    case AnimationState::WalkLeft: 
                        m_pAnimator->PlayAnimation(6, 3); 
                        break;
                    case AnimationState::WalkUp: 
                        m_pAnimator->PlayAnimation(9, 3); 
                        break; // reuse down for now
                    }
                }
            }
        }

    private:
        GameObject* m_Character;
        glm::vec3 m_Direction;
        float m_Speed;

        SpriteAnimatorComponent* m_pAnimator{};
        AnimationState m_AnimState{ AnimationState::WalkDown };
    };

    class StopAnimationCommand : public Command
    {
    public:
        StopAnimationCommand(SpriteAnimatorComponent* animator)
            : m_pAnimator(animator) {}

        void Execute() override
        {
            if (m_pAnimator)
            {
                //std::cout << "Stop animation" << std::endl;
                m_pAnimator->Stop();
            }
        }

    private:
        SpriteAnimatorComponent* m_pAnimator;
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

    //needs to react to something, like pick up pellets, so make this a add pellets
    class AddScoreCommand : public Command
    {
    public:
        explicit AddScoreCommand(ScoreComponent* score, int points)
            : m_pScoreComponent(score), m_Points(points) {}

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