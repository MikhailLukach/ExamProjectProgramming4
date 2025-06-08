#include "LevelManagerComponent.h"
#include "TileTrackerComponent.h"
#include "GameObject.h"

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
}
