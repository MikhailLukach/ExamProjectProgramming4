#include "LevelResetComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LivesComponent.h"
#include "ScoreComponent.h"
#include "PersistentPlayerStats.h"



dae::LevelResetComponent::LevelResetComponent(std::function<void(int)> reloadFunc, bool isCoop)
    : m_ReloadFunc(std::move(reloadFunc))
    , m_DeathThreshold(isCoop ? 2 : 1)
{
}

void dae::LevelResetComponent::Update(float deltaTime)
{
    if (!m_Waiting) return;

    m_Timer -= deltaTime;
    if (m_Timer > 0.0f) return;

    m_Waiting = false;

    auto* scene = dae::SceneManager::GetInstance().GetCurrentScene();
    if (scene)
    {
        // Player 1
        auto player1 = scene->FindObjectWithComponent<LivesComponent>();
        if (auto lives = player1 ? player1->GetComponent<LivesComponent>() : nullptr)
            dae::g_Player1Stats.Lives = lives->GetLives();

        auto scoreObj1 = scene->FindObjectWithComponent<ScoreComponent>();
        if (auto score = scoreObj1 ? scoreObj1->GetComponent<ScoreComponent>() : nullptr)
            dae::g_Player1Stats.Score = score->GetScore();

        // Player 2
        auto allPlayers = scene->FindObjectsWithComponent<LivesComponent>();
        if (allPlayers.size() > 1)
        {
            if (auto lives = allPlayers[1]->GetComponent<LivesComponent>())
                dae::g_Player2Stats.Lives = lives->GetLives();

            if (auto score = allPlayers[1]->GetComponent<ScoreComponent>())
                dae::g_Player2Stats.Score = score->GetScore();
        }
    }

    SceneManager::GetInstance().RequestReload([target = m_TargetLevel, func = m_ReloadFunc]() {
        func(target);
        });
}

void dae::LevelResetComponent::Notify(EventId event, GameObject*)
{
    if (event != EventId::PLAYER_DIED || m_Waiting)
    {
        return;
    }

    ++m_DeathsSeen;
    std::cout << "[LevelResetComponent] Deaths seen increased to: " << m_DeathsSeen << " required deaths: " << m_DeathThreshold << std::endl;
    if (m_DeathsSeen >= m_DeathThreshold)
    {
        m_Waiting = true;
        m_Timer = kDelay;
    }
}

void dae::LevelResetComponent::QueueResetToLevel(int levelIndex)
{
    m_TargetLevel = levelIndex;
    m_Waiting = true;
    m_Timer = kDelay;
}
