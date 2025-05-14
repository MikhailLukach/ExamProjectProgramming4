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
