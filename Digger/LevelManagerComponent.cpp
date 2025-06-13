#include "LevelManagerComponent.h"
#include "TileTrackerComponent.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GemComponent.h"
#include "LevelResetComponent.h"
#include "NobbinControllerComponent.h"
#include <SoundServiceLocator.h>

dae::LevelManagerComponent::LevelManagerComponent(int levelIndex)
    :m_CurrentLevelIndex(levelIndex)
{
}

void dae::LevelManagerComponent::RegisterMoneyBag(std::shared_ptr<GameObject> moneyBag)
{
	if (moneyBag)
	{
		m_MoneyBags.push_back(moneyBag);
	}
}

void dae::LevelManagerComponent::RegisterNobbin(std::shared_ptr<GameObject> nobbin)
{
    if (nobbin) m_Nobbins.push_back(nobbin);
}

std::shared_ptr<dae::GameObject> dae::LevelManagerComponent::GetMoneyBagAt(int col, int row) const
{
    for (const auto& bag : m_MoneyBags)
    {
        if (!bag) continue;

        auto tracker = bag->GetComponent<TileTrackerComponent>();

        if (!tracker) continue;

        glm::ivec2 tile = tracker->GetTileCoords();
        if (tile.x == col && tile.y == row)
            return bag;
    }

    return nullptr;
}

std::shared_ptr<dae::GameObject> dae::LevelManagerComponent::GetNobbinAt(int col, int row) const
{
    for (const auto& nobbin : m_Nobbins)
    {
        if (!nobbin) continue;
        auto tracker = nobbin->GetComponent<TileTrackerComponent>();
        if (!tracker) continue;

        auto tile = tracker->GetTileCoords();
        if (tile.x == col && tile.y == row)
            return nobbin;
    }
    return nullptr;
}

void dae::LevelManagerComponent::Update(float deltaTime)
{
    (void)deltaTime;
    for (const auto& bag : m_MoneyBags)
    {
        if (!bag) continue;
        auto tracker = bag->GetComponent<TileTrackerComponent>();
        if (!tracker) continue;

        glm::ivec2 tile = tracker->GetTileCoords();
        //std::cout << "[LevelManager] MoneyBag at tile: (" << tile.x << ", " << tile.y << ")\n";
    }

    auto* scene = dae::SceneManager::GetInstance().GetCurrentScene();
    if (!scene || m_LoadedNextLevel) return;

    auto gems = scene->FindObjectsWithComponent<GemComponent>();
    const int remainingGems = static_cast<int>(gems.size());

    //std::cout << "[LevelManagerComponent] Remaining gems: " << remainingGems << std::endl;

    if (remainingGems == 0)
    {
        auto nobbins = scene->FindObjectsWithComponent<NobbinControllerComponent>();
        for (const auto& obj : nobbins)
        {
            if (auto ctrl = obj->GetComponent<NobbinControllerComponent>())
            {
                ctrl->SetSpeed(0.f);
            }
        }

        dae::SoundServiceLocator::Get().PlaySound(dae::ResourceManager::GetInstance().GetFullPath("LevelComplete.wav"));

        ++m_CurrentLevelIndex;
        char buf[64];
        std::snprintf(buf, sizeof(buf), m_LevelFilePattern.c_str(), m_CurrentLevelIndex);

        auto resetObj = scene->FindObjectWithComponent<LevelResetComponent>();
        if (auto reset = resetObj ? resetObj->GetComponent<LevelResetComponent>() : nullptr)
        {
            reset->QueueResetToLevel(m_CurrentLevelIndex);
        }

        m_LoadedNextLevel = true;

        std::cout << "[LevelManager] Loaded next level: " << buf << "\n";

        // m_pLoader->LoadLevelBinary(buf, ...);
    }
}
