#include "NobbinSpawnerComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "RenderComponent.h"
#include "SpriteAnimatorComponent.h"
#include "TileTrackerComponent.h"
#include "TileManagerComponent.h"
#include "NobbinComponent.h"
#include "NobbinControllerComponent.h"
#include "LevelLoader.h"
#include "LevelManagerComponent.h"
#include "NobbinState.h"
#include "LivesComponent.h"

dae::NobbinSpawnerComponent::NobbinSpawnerComponent(Scene* scene, LevelManagerComponent* levelMgr, LevelLoader* loader
    , TileManagerComponent* tileManager, GameObject* player, int tileX, int tileY, float spawnDelay, int maxNobbins)
	: m_Scene(scene), m_LevelManager(levelMgr), m_LevelLoader(loader), m_TileManager(tileManager), m_Player(player),
	m_SpawnTile(tileX, tileY), m_SpawnDelay(spawnDelay), m_MaxNobbins(maxNobbins), m_Timer(0.0f)
{
}

void dae::NobbinSpawnerComponent::Update(float deltaTime)
{
    if (m_IsPaused)
    {
        m_PauseTimer -= deltaTime;
        if (m_PauseTimer <= 0.f)
        {
            KillAllNobbins();
            m_IsPaused = false;
            m_Timer = m_SpawnDelay; // ready to spawn immediately next frame
        }
        return;
    }

    m_LiveNobbins.erase(std::remove_if(m_LiveNobbins.begin(), m_LiveNobbins.end(),
        [](const std::weak_ptr<GameObject>& wptr) { return wptr.expired(); }),
        m_LiveNobbins.end());

    // Spawn logic
    if (static_cast<int>(m_LiveNobbins.size()) < m_MaxNobbins)
    {
        m_Timer += deltaTime; 

        if (m_Timer >= m_SpawnDelay)
        {
            SpawnNobbin();
            m_Timer = 0.0f;
        }
    }
}

void dae::NobbinSpawnerComponent::Notify(EventId event, GameObject*)
{
    if (event == EventId::PlAYER_HIT || event == EventId::PLAYER_DIED)
    {
        // Freeze and kill all current Nobbins
        m_IsPaused = true;
        m_PauseTimer = m_SpawnDelay;  // pause for same delay
        for (auto& wptr : m_LiveNobbins)
        {
            if (auto sp = wptr.lock())
            {
                if (auto ctrl = sp->GetComponent<NobbinControllerComponent>())
                {
                    ctrl->SetSpeed(0.f);
                }
            }        
        }
    }
}

void dae::NobbinSpawnerComponent::SpawnNobbin()
{
    auto nobbin = std::make_shared<GameObject>();
    auto spawnPos = m_LevelLoader->GetWorldCenterForTile(m_SpawnTile.x, m_SpawnTile.y);
    nobbin->GetTransform()->SetPosition(spawnPos);

    auto render = nobbin->AddComponent<RenderComponent>("NormalNobbinSpritesheet.png");
    render->SetSize(32, 32);
    render->SetRenderOffset(glm::vec2{ 0.f, -16.f });

    auto animator = nobbin->AddComponent<SpriteAnimatorComponent>(render.get(), 16, 16, 0.15f);
    animator->PlayAnimation(6, 3);

    nobbin->AddComponent<TileTrackerComponent>(42, 43, 5, 48);
    nobbin->AddComponent<NobbinComponent>();
    nobbin->AddComponent<NobbinControllerComponent>(m_Player, m_TileManager, m_LevelManager, m_LevelLoader, 0.05f, 100.f);

    m_LevelManager->RegisterNobbin(nobbin);
    m_Scene->Add(nobbin);

    m_LiveNobbins.emplace_back(nobbin);

    std::cout << "[Spawner] Spawned Nobbin. Total: " << m_LiveNobbins.size() << "\n";
}

void dae::NobbinSpawnerComponent::KillAllNobbins()
{
    for (auto& wptr : m_LiveNobbins)
    {
        if (auto sp = wptr.lock())
        {
            sp->MarkForDeletion();
        }
    }

    m_LiveNobbins.clear();
}
