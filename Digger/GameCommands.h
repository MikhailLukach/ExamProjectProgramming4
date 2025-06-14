#pragma once
#include <iostream>
#include "GameObject.h"
#include "HealthComponent.h"
#include "ScoreComponent.h"
#include "FireBallComponent.h"
#include "SpriteAnimatorComponent.h"
#include "TileManagerComponent.h"
#include "AnimationState.h"
#include "SoundServiceLocator.h"
#include "ISoundSystem.h"
#include "GridSettings.h"
#include <CollisionHelper.h>

namespace dae
{
    class MoveCommand : public Command
    {
    public:
        MoveCommand(GameObject* character, const glm::vec3& direction, float speed,
            TileManagerComponent* tileManager = nullptr,
            LevelManagerComponent* levelManager = nullptr,
            SpriteAnimatorComponent* animator = nullptr, AnimationState state = AnimationState::WalkDown)
            : m_Character(character), m_Direction(direction), m_Speed(speed), m_pTileManager(tileManager),
             m_pLevelManager(levelManager), m_pAnimator(animator), m_AnimState(state) {}

        void Execute() override
        {
            if (!m_Character) return;

            if (!dae::InputManager::GetInstance().IsInputEnabled(m_Character))
                return;

            auto transform = m_Character->GetTransform();
            auto tracker = m_Character->GetComponent<TileTrackerComponent>();
            if (!tracker) return;

            glm::vec3 pos = transform->GetWorldPosition();
            glm::ivec2 tile = tracker->GetTileCoords();

            // Compute center of current tile
            float centerX = tile.x * GridSettings::TileWidth + GridSettings::TileWidth / 2.0f + GridSettings::GridOffsetX;
            float centerY = tile.y * GridSettings::TileHeight + GridSettings::TileHeight / 2.0f + GridSettings::GridOffsetY;
            glm::vec3 center{ centerX + 5.0f, centerY + 5.0f, 0.0f }; // 5px sprite visual adjustment

            glm::vec3 toCenter = center - pos;
            float threshold = 1.0f;

            glm::vec3 desiredDir = glm::normalize(m_Direction);
            s_LastDirection = desiredDir;

            glm::vec3 orientedPos{pos};
            if (desiredDir.x < 0 || desiredDir.y < 0)
            {
                orientedPos = glm::vec3{ pos.x - 25, pos.y - 25, pos.z };
            }
            if(desiredDir.x > 0)
            {
                orientedPos = glm::vec3{ pos.x + 15, pos.y, pos.z };
            }
            if(desiredDir.y > 0)
            {
                orientedPos = glm::vec3{ pos.x, pos.y + 15, pos.z };
            }
            glm::vec3 predictedPosition = orientedPos + desiredDir * m_Speed;

            int predictedTileX = static_cast<int>(std::floor((predictedPosition.x - GridSettings::GridOffsetX) / GridSettings::TileWidth));
            int predictedTileY = static_cast<int>(std::floor((predictedPosition.y - GridSettings::GridOffsetY) / GridSettings::TileHeight));

            bool nextTileIsOutOfBounds =
                predictedTileX < 0 || predictedTileX >= GridSettings::NumCols ||
                predictedTileY < 0 || predictedTileY >= GridSettings::NumRows;

            glm::vec3 velocity{};

            if (nextTileIsOutOfBounds)
            {
                // If already centered and trying to move out of bounds  don't move
                if (glm::length(toCenter) < threshold)
                {
                    return;
                }

                // If not centered, align to center first on perpendicular axis
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
                    return;
                }
            }
            else
            {
                if (auto render = m_Character->GetComponent<RenderComponent>())
                {
                    render->ResetRenderOffset();
                }
                // Movement allowed within bounds
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
            }

            if (m_pLevelManager && desiredDir.y == 0.f && desiredDir.x != 0.f)
            {
                int pushDir = static_cast<int>(glm::sign(desiredDir.x));

                glm::ivec2 playerTile = tracker->GetTileCoords();
                glm::ivec2 targetTile = playerTile + glm::ivec2{ pushDir, 0 };

                auto bagObj = m_pLevelManager->GetMoneyBagAt(targetTile.x, targetTile.y);
                if (bagObj)
                {
                    glm::vec3 playerPos = transform->GetWorldPosition();
                    glm::vec3 bagPos = bagObj->GetTransform()->GetWorldPosition();
                    float distance = std::abs(playerPos.x - bagPos.x);

                    if (distance < 34.0f)
                    {
                        auto bagComp = bagObj->GetComponent<MoneyBagComponent>();
                        if (bagComp)
                        {
                            bagComp->TryPushHorizontally(pushDir, m_pTileManager);
                        }
                    }
                    else
                    {
                        std::cout << "[MoveCommand] Skipped push: player too far (distance: " << distance << ")\n";
                    }
                }
            }

            if (m_pLevelManager && desiredDir.y != 0.f && desiredDir.x == 0.f)
            {
                glm::vec3 offset;
                if(desiredDir.y == 1.f)
                {
                    offset = glm::vec3{ -16.f, 16.f, 0.f };
                }
                else
                {
                    offset = glm::vec3{ -16.f, -16.f, 0.f };
                }
                glm::vec3 predictedPosMoneyBag = pos + offset + desiredDir * m_Speed;
                int predictedTileXMoneyBag = static_cast<int>((predictedPosMoneyBag.x - GridSettings::GridOffsetX + 16.0f) / GridSettings::TileWidth); 
                int predictedTileYMoneyBag = static_cast<int>((predictedPosMoneyBag.y - GridSettings::GridOffsetY) / GridSettings::TileHeight);

                auto blockingBag = m_pLevelManager->GetMoneyBagAt(predictedTileXMoneyBag, predictedTileYMoneyBag);
                if (blockingBag)
                {
                    auto bagComp = blockingBag->GetComponent<MoneyBagComponent>();
                    if (bagComp && bagComp->IsIdle())
                    {
                        std::cout << "[MoveCommand] Movement blocked by idle MoneyBag at (" << predictedTileX << ", " << predictedTileY << ")\n";
                        return;
                    }
                }
            }

            transform->SetPosition(pos + velocity);

            if (m_pLevelManager)
            {
                auto playerTile = tracker->GetTileCoords();
                auto bag = m_pLevelManager->GetMoneyBagAt(playerTile.x, playerTile.y - 1);
                if (bag)
                {
                    auto bagComp = bag->GetComponent<MoneyBagComponent>();
                    if (bagComp)
                    {
                        bagComp->SetPlayerBelow();
                    }
                }
            }

            if (!m_pTileManager)
            {
                std::cout << "[MoveCommand] No LevelManager assigned, skipping dig check.\n";
                return;
            }

            if (tracker = m_Character->GetComponent<TileTrackerComponent>())
            {
                glm::ivec2 coords = tracker->GetTileCoords();
                int col = coords.x;
                int row = coords.y;

                auto tileObj = m_pTileManager->GetTileAt(col, row);
                if (tileObj)
                {
                    auto tileComp = tileObj->GetComponent<TileComponent>();
                    if (tileComp)
                    {
                        tileComp->Dig();
                    }
                    else
                    {
                        std::cout << "[MoveCommand] Tile at (" << col << ", " << row << ") has no TileComponent!\n";
                    }
                }
            }
        }

        static glm::vec3 GetLastDirection() { return s_LastDirection; }

    private:
        GameObject* m_Character;
        glm::vec3 m_Direction;
        float m_Speed;

        SpriteAnimatorComponent* m_pAnimator{};
        TileManagerComponent* m_pTileManager{};
        LevelManagerComponent* m_pLevelManager{};
        AnimationState m_AnimState{ AnimationState::WalkDown };

        inline static glm::vec3 s_LastDirection{ 1,0,0 };
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

    class ShootFireballCommand : public Command
    {
    public:
        ShootFireballCommand(GameObject* player, TileManagerComponent* tileManager, ScoreComponent* scoreComp, float cooldown = 10.f)
            : m_Player(player), m_CooldownMs(static_cast<Uint32>(cooldown * 1000)), m_pTileManager(tileManager), m_pScore(scoreComp)
        {
        }

        void Execute() override
        {
            std::cout << "Fire!\n";
            Uint32 now = SDL_GetTicks();
            if (now < s_NextFireAllowed)
            {
                return;
            }

            s_NextFireAllowed = now + m_CooldownMs;

            // **Pull the direction from MoveCommand�s static**
            glm::vec3 dir3 = MoveCommand::GetLastDirection();
            glm::vec2 dir{ dir3.x, dir3.y };

            // Spawn the fireball in that direction�
            auto fire = std::make_shared<GameObject>();
            glm::vec3 spawnPos = m_Player->GetTransform()->GetWorldPosition();
            fire->GetTransform()->SetWorldPosition(spawnPos + glm::vec3(dir * 16.0f, 0));

            auto renderFireBall = fire->AddComponent<RenderComponent>("FireballAnim.png");
            renderFireBall->SetSize(32, 32);

            auto animator = fire->AddComponent<SpriteAnimatorComponent>(renderFireBall.get(), 16, 16, 0.1f);
            animator->PlayAnimation(0, 2, true);

            fire->AddComponent<FireBallComponent>(dir, 200.f, 2.f, m_pTileManager, m_pScore);

            SceneManager::GetInstance().GetCurrentScene()->Add(fire);
        }

    private:
        GameObject* m_Player;
        TileManagerComponent* m_pTileManager;
        ScoreComponent* m_pScore;
        Uint32 m_CooldownMs;
        static Uint32 s_NextFireAllowed;
    };

    Uint32 ShootFireballCommand::s_NextFireAllowed = 0;

    class ShootFireballCommandIndividual : public Command
    {
    public:
        ShootFireballCommandIndividual(GameObject* player,
            TileManagerComponent* tileManager,
            ScoreComponent* scoreComp,
            float cooldownSeconds = 10.f)
            : m_Player{ player }
            , m_pTileManager{ tileManager }
            , m_pScore{ scoreComp }
            , m_CooldownMs{ static_cast<Uint32>(cooldownSeconds * 1000) }
            , m_NextFireAllowed{ 0 }
        {
        }

        void Execute() override
        {
            Uint32 now = SDL_GetTicks();
            if (now < m_NextFireAllowed)
                return;                       

            m_NextFireAllowed = now + m_CooldownMs;

            auto tracker = m_Player->GetComponent<TileTrackerComponent>();
            glm::vec3 dir3 = MoveCommand::GetLastDirection();
            glm::vec2 dir{ dir3.x, dir3.y };

            auto fire = std::make_shared<GameObject>();
            glm::vec3 spawnPos = m_Player->GetTransform()->GetWorldPosition();
            fire->GetTransform()->SetWorldPosition(spawnPos + glm::vec3(dir * 16.f, 0.f));

            auto render = fire->AddComponent<RenderComponent>("FireballAnim.png");
            render->SetSize(32, 32);

            auto animator = fire->AddComponent<SpriteAnimatorComponent>(
                render.get(), 16, 16, 0.1f);
            animator->PlayAnimation(0, 2, true);

            fire->AddComponent<FireBallComponent>(dir, 200.f, 2.f, m_pTileManager, m_pScore);

            SceneManager::GetInstance().GetCurrentScene()->Add(fire);
        }

    private:
        GameObject* m_Player;
        TileManagerComponent* m_pTileManager;
        ScoreComponent* m_pScore;
        Uint32 m_CooldownMs;
        Uint32 m_NextFireAllowed;  
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

    class SkipLevelCommand : public Command
    {
    public:
        explicit SkipLevelCommand(LevelManagerComponent* levelManager)
            : m_pLevelManager(levelManager) {
        }

        void Execute() override
        {
            if (m_pLevelManager)
            {
                m_pLevelManager->LoadNextLevelOrScoreboard(); // Will trigger scoreboard if at level 3
            }
        }

    private:
        LevelManagerComponent* m_pLevelManager;
    };

    class ToggleMuteCommand : public Command
    {
    public:
        void Execute() override
        {
            auto& soundSystem = dae::SoundServiceLocator::Get();
            m_IsMuted = !m_IsMuted;
            soundSystem.SetMuted(m_IsMuted);
            std::cout << "[Sound] " << (m_IsMuted ? "Muted" : "Unmuted") << "\n";
        }

    private:
       bool m_IsMuted = false;
    };

    class MoveCommandNobbin : public Command
    {
    public:
        MoveCommandNobbin(GameObject* character,
            const glm::vec3& direction,
            float speed,
            TileManagerComponent* tileManager,
            LevelManagerComponent* levelManager,
            SpriteAnimatorComponent* animator,
            AnimationState state)
            : m_Character(character)
            , m_Direction(direction)
            , m_Speed(speed)
            , m_pTileManager(tileManager)
            , m_pLevelManager(levelManager)
            , m_pAnimator(animator)
            , m_AnimState(state)
        {
        }

        void Execute() override
        {
            if (!m_Character) return;

            if (!dae::InputManager::GetInstance().IsInputEnabled(m_Character))
                return;

            auto transform = m_Character->GetTransform();
            auto tracker = m_Character->GetComponent<TileTrackerComponent>();
            if (!tracker) return;

            glm::vec3 pos = transform->GetWorldPosition();
            glm::ivec2 tile = tracker->GetTileCoords();

            // Compute center of current tile
            float centerX = tile.x * GridSettings::TileWidth + GridSettings::TileWidth / 2.0f + GridSettings::GridOffsetX;
            float centerY = tile.y * GridSettings::TileHeight + GridSettings::TileHeight / 2.0f + GridSettings::GridOffsetY;
            glm::vec3 center{ centerX + 5.0f, centerY + 5.0f, 0.0f }; // 5px sprite visual adjustment

            glm::vec3 toCenter = center - pos;
            float threshold = 1.0f;

            glm::vec3 desiredDir = glm::normalize(m_Direction);
            s_LastDirectionNobbin = desiredDir;

            glm::vec3 orientedPos{ pos };
            if (desiredDir.x < 0 || desiredDir.y < 0)
            {
                orientedPos = glm::vec3{ pos.x - 25, pos.y - 25, pos.z };
            }
            if (desiredDir.x > 0)
            {
                orientedPos = glm::vec3{ pos.x + 15, pos.y, pos.z };
            }
            if (desiredDir.y > 0)
            {
                orientedPos = glm::vec3{ pos.x, pos.y + 15, pos.z };
            }
            glm::vec3 predictedPosition = orientedPos + desiredDir * m_Speed;

            int predictedTileX = static_cast<int>(std::floor((predictedPosition.x - GridSettings::GridOffsetX) / GridSettings::TileWidth));
            int predictedTileY = static_cast<int>(std::floor((predictedPosition.y - GridSettings::GridOffsetY) / GridSettings::TileHeight));

            if (auto du = m_Character->GetComponent<DigUnlockComponent>())
            {
                m_CanDig = du->CanDig();
            }

            glm::ivec2 nextTile{ predictedTileX, predictedTileY };
            if (m_pTileManager && !m_pTileManager->IsDugTile(nextTile) && !m_CanDig)
            {
                return;
            }

            bool nextTileIsOutOfBounds =
                predictedTileX < 0 || predictedTileX >= GridSettings::NumCols ||
                predictedTileY < 0 || predictedTileY >= GridSettings::NumRows;

            glm::vec3 velocity{};

            if (nextTileIsOutOfBounds)
            {
                // If already centered and trying to move out of bounds  don't move
                if (glm::length(toCenter) < threshold)
                {
                    return;
                }

                // If not centered, align to center first on perpendicular axis
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
                    return;
                }
            }
            else
            {
                if (auto render = m_Character->GetComponent<RenderComponent>())
                {
                    render->ResetRenderOffset();
                }
                // Movement allowed within bounds
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
            }

            if (m_pLevelManager && desiredDir.y != 0.f && desiredDir.x == 0.f)
            {
                glm::vec3 offset;
                if (desiredDir.y == 1.f)
                {
                    offset = glm::vec3{ -16.f, 16.f, 0.f };
                }
                else
                {
                    offset = glm::vec3{ -16.f, -16.f, 0.f };
                }
                glm::vec3 predictedPosMoneyBag = pos + offset + desiredDir * m_Speed;
                int predictedTileXMoneyBag = static_cast<int>((predictedPosMoneyBag.x - GridSettings::GridOffsetX + 16.0f) / GridSettings::TileWidth);
                int predictedTileYMoneyBag = static_cast<int>((predictedPosMoneyBag.y - GridSettings::GridOffsetY) / GridSettings::TileHeight);

                auto blockingBag = m_pLevelManager->GetMoneyBagAt(predictedTileXMoneyBag, predictedTileYMoneyBag);
                if (blockingBag)
                {
                    auto bagComp = blockingBag->GetComponent<MoneyBagComponent>();
                    if (bagComp && bagComp->IsIdle())
                    {
                        std::cout << "[MoveCommand] Movement blocked by idle MoneyBag at (" << predictedTileX << ", " << predictedTileY << ")\n";
                        return;
                    }
                }
            }

            transform->SetPosition(pos + velocity);

            if (tracker = m_Character->GetComponent<TileTrackerComponent>())
            {
                if (m_CanDig)
                {
                    glm::ivec2 coords = tracker->GetTileCoords();
                    int col = coords.x;
                    int row = coords.y;

                    auto tileObj = m_pTileManager->GetTileAt(col, row);
                    if (tileObj)
                    {
                        auto tileComp = tileObj->GetComponent<TileComponent>();
                        if (tileComp)
                        {
                            tileComp->Dig();
                        }
                        else
                        {
                            std::cout << "[MoveCommand] Tile at (" << col << ", " << row << ") has no TileComponent!\n";
                        }
                    }
                }
            }

            if (m_CanDig)
            {
                auto* scene = dae::SceneManager::GetInstance().GetCurrentScene();
                if (!scene) return;

                auto* owner = m_Character;
                if (!owner) return;

                for (const auto& obj : scene->GetObjects())
                {
                    if (!obj || obj.get() == owner) continue;

                    // Gem logic
                    if (auto gem = obj->GetComponent<dae::GemComponent>())
                    {
                        if (dae::CheckRenderComponentCollision(owner, obj.get()))
                        {
                            std::cout << "[ChasingAndDiggingState] Destroyed Gem!\n";
                            obj->MarkForDeletion();
                        }
                        continue; // don't treat gem as moneybag
                    }

                    // MoneyBag logic
                    if (auto moneyBag = obj->GetComponent<dae::MoneyBagComponent>())
                    {
                        if (!moneyBag->IsFalling()) // Ensure only grounded bags
                        {
                            if (dae::CheckRenderComponentCollision(owner, obj.get()))
                            {
                                std::cout << "[ChasingAndDiggingState] Destroyed MoneyBag!\n";
                                obj->MarkForDeletion();
                            }
                        }
                    }
                }
            }
        }

        static glm::vec3 GetLastDirection() { return s_LastDirectionNobbin; }

    private:
        GameObject* m_Character;
        glm::vec3 m_Direction;
        float m_Speed;
        bool m_CanDig = false;
        TileManagerComponent* m_pTileManager;
        SpriteAnimatorComponent* m_pAnimator;
        LevelManagerComponent* m_pLevelManager;
        AnimationState m_AnimState;
        inline static glm::vec3 s_LastDirectionNobbin{ 1,0,0 };
    };
}