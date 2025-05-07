#pragma once
#include <iostream>
#include "GameObject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "SpriteAnimatorComponent.h"
#include "AnimationState.h"
#include "SoundServiceLocator.h"
#include "ISoundSystem.h"
#include "GridSettings.h"

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
            if (!m_Character) return;

            auto transform = m_Character->GetTransform();
            auto tracker = m_Character->GetComponent<TileTrackerComponent>();
            if (!tracker) return;

            glm::vec3 pos = transform->GetWorldPosition();
            glm::ivec2 tile = tracker->GetTileCoords();

            // Predict which tile we would move to
            glm::vec3 desiredDir = glm::normalize(m_Direction);
            glm::vec3 predictedPosition = pos + desiredDir * m_Speed;

            // Convert predicted position to tile coordinates
            int predictedTileX = static_cast<int>(std::floor((predictedPosition.x - GridSettings::GridOffsetX) / GridSettings::TileWidth));
            int predictedTileY = static_cast<int>(std::floor((predictedPosition.y - GridSettings::GridOffsetY) / GridSettings::TileHeight));

            // Clamp movement within grid
            if (predictedTileX < 0 || predictedTileX >= GridSettings::NumCols ||
                predictedTileY < 0 || predictedTileY >= GridSettings::NumRows)
            {
                return;
            }

            // Calculate center of current tile
            float centerX = tile.x * GridSettings::TileWidth + GridSettings::TileWidth / 2.0f + GridSettings::GridOffsetX;
            float centerY = tile.y * GridSettings::TileHeight + GridSettings::TileHeight / 2.0f + GridSettings::GridOffsetY;
            glm::vec3 center{ centerX + 5.0f, centerY + 5.0f, 0.0f }; 

            glm::vec3 toCenter = center - pos;
            float threshold = 1.0f;

            desiredDir = glm::normalize(m_Direction);
            glm::vec3 velocity{};

            if (desiredDir.x != 0.f && std::abs(toCenter.y) > threshold)
            {
                velocity = glm::normalize(glm::vec3(0, toCenter.y, 0)) * m_Speed;
            }
            else if (desiredDir.y != 0.f && std::abs(toCenter.x) > threshold)
            {
                velocity = glm::normalize(glm::vec3(toCenter.x, 0, 0)) * m_Speed;
            }
            else
            {
                velocity = desiredDir * m_Speed;

                if (m_pAnimator)
                {
                    switch (m_AnimState)
                    {
                    case AnimationState::WalkRight: m_pAnimator->PlayAnimation(0, 3); break;
                    case AnimationState::WalkDown:  m_pAnimator->PlayAnimation(3, 3); break;
                    case AnimationState::WalkLeft:  m_pAnimator->PlayAnimation(6, 3); break;
                    case AnimationState::WalkUp:    m_pAnimator->PlayAnimation(9, 3); break;
                    }
                }
            }

            transform->SetPosition(pos + velocity);
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
            //std::cout << "Release animation" << std::endl;
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

    class PlaySoundCommand : public Command
    {
    public:
        explicit PlaySoundCommand(const std::string& soundFilePath)
            : m_SoundFile(soundFilePath) {}

        void Execute() override
        {
            ISoundSystem& soundSystem = SoundServiceLocator::Get();
            std::cout << "Trying to play sound" << std::endl;
            soundSystem.PlaySound(m_SoundFile);
        }

    private:
        std::string m_SoundFile;
    };
}