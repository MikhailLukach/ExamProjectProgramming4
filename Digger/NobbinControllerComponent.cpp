#include "NobbinControllerComponent.h"
#include "GameObject.h"
#include "TileManagerComponent.h"
#include "TileTrackerComponent.h"
#include "NobbinComponent.h"
#include "TileComponent.h"
#include "AIUtils.h"
#include "TileTypes.h"
#include "LevelLoader.h"
#include <glm.hpp>
#include <memory>

dae::NobbinControllerComponent::NobbinControllerComponent(GameObject* player,
	TileManagerComponent* tileManager,
	LevelLoader* levelLoader,
	float decisionInterval, float speed)
	: m_pPlayer(player)
	, m_pTileManager(tileManager)
	, m_pLevelLoader(levelLoader)
	, m_DecisionInterval(decisionInterval)
	, m_Speed(speed)
{
}

float m_PostMoveDelay = 0.05f;
float m_PostMoveTimer = 0.f;

void dae::NobbinControllerComponent::Update(float deltaTime)
{
	(void)deltaTime;
	if (!m_pTracker)
		m_pTracker = GetOwner()->GetComponent<TileTrackerComponent>().get();

	if (!m_pPlayer || !m_pTileManager || !m_pTracker) return;

	auto mover = GetOwner()->GetComponent<NobbinComponent>();
	if (!mover || mover->NobbinIsMoving()) return;

	const auto myTile = m_pTracker->GetTileCoords();
	const auto playerTracker = m_pPlayer->GetComponent<TileTrackerComponent>();
	if (!playerTracker) return;

	const auto playerTile = playerTracker->GetTileCoords();

	std::vector<glm::ivec2> directions = {
		{1, 0}, {-1, 0}, {0, 1}, {0, -1}
	};

	if (mover->NobbinIsMoving())
	{
		m_PostMoveTimer = m_PostMoveDelay; // reset
		return;
	}

	if (m_PostMoveTimer > 0.f)
	{
		m_PostMoveTimer -= deltaTime;
		return;
	}

	// Sort directions by increasing Manhattan distance to player
	std::sort(directions.begin(), directions.end(), [&](const glm::ivec2& a, const glm::ivec2& b) {
		glm::ivec2 posA = myTile + a;
		glm::ivec2 posB = myTile + b;
		int distA = glm::abs(posA.x - playerTile.x) + glm::abs(posA.y - playerTile.y);
		int distB = glm::abs(posB.x - playerTile.x) + glm::abs(posB.y - playerTile.y);
		return distA < distB;
		});

	for (const auto& dir : directions)
	{
		glm::ivec2 nextTile = myTile + dir;

		// Skip if we're backtracking
		if (nextTile == m_PreviousTile)
		{
			//std::cout << "[DEBUG] Skipping backtracking to (" << nextTile.x << ", " << nextTile.y << ")\n";
			continue;
		}

		// Move if it's a dug tile
		if (IsDugTile(nextTile))
		{
			TryMoveInDirection(dir);
			return;
		}
	}

	//std::cout << "[Nobbin] No valid move found from tile (" << myTile.x << ", " << myTile.y << ")\n";
}

void dae::NobbinControllerComponent::TryMoveInDirection(const glm::ivec2& direction)
{
	auto tracker = GetOwner()->GetComponent<TileTrackerComponent>();
	auto nobbinMover = GetOwner()->GetComponent<NobbinComponent>();
	if (!tracker || !nobbinMover || nobbinMover->NobbinIsMoving()) return;

	std::cout << "[DEBUG] Attempting move while moving? " << nobbinMover->NobbinIsMoving() << "\n";

	auto currentTile = tracker->GetTileCoords();
	glm::ivec2 nextTile = currentTile + direction;

	if (!IsDugTile(nextTile)) return;

	glm::vec3 worldPos = m_pLevelLoader->GetWorldCenterForTile(nextTile.x, nextTile.y);

	float distance = glm::distance(tracker->GetOwner()->GetTransform()->GetWorldPosition(), worldPos);
	float duration = distance / m_Speed;

	nobbinMover->NobbinStartMoveTo(worldPos, duration);

	//std::cout << "[Nobbin] Moving from (" << currentTile.x << "," << currentTile.y
		//<< ") to (" << nextTile.x << "," << nextTile.y << ")\n";

	m_PreviousTile = currentTile;
}

bool dae::NobbinControllerComponent::IsDugTile(const glm::ivec2& tile) const
{
	if (!m_pTileManager) return false;

	auto tileObj = m_pTileManager->GetTileAt(tile.x, tile.y);
	if (!tileObj) return false;

	auto tileComp = tileObj->GetComponent<TileComponent>();
	if (!tileComp) return false;

	return tileComp->GetType() == TileVisualType::Dug_Spot;
}
