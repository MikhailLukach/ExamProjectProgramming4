#include "NobbinControllerComponent.h"
#include "TileManagerComponent.h"
#include "LevelManagerComponent.h"
#include "TileTrackerComponent.h"
#include "SpriteAnimatorComponent.h"
#include "MoneyBagComponent.h"
#include "NobbinComponent.h"
#include "TileComponent.h"
#include "SceneManager.h"
#include "Scene.h"
#include "LivesComponent.h"
#include "AIUtils.h"
#include "TileTypes.h"
#include "LevelLoader.h"
#include "NobbinState.h"
#include "ChasingState.h"
#include "ChasingAndDiggingState.h"
#include "GettingCrushedState.h"

#include <vector>
#include <array>
#include <algorithm>
#include <glm.hpp>
#include <memory>

dae::NobbinControllerComponent::NobbinControllerComponent(GameObject* player,
	TileManagerComponent* tileManager,
	LevelManagerComponent* levelManager,
	LevelLoader* levelLoader,
	float decisionInterval, float speed)
	: m_pPlayer(player)
	, m_pTileManager(tileManager)
	, m_pLevelManager(levelManager)
	, m_pLevelLoader(levelLoader)
	, m_DecisionInterval(decisionInterval)
	, m_Speed(speed)
{
	ChangeState(std::make_unique<ChasingState>());
}

void dae::NobbinControllerComponent::Update(float deltaTime)
{
	(void)deltaTime;
	auto nobbinBounds = GetOwner()->GetTransform()->GetWorldPosition(); // center
	for (const auto& bag : m_pLevelManager->GetAllMoneyBags())
	{
		if (!bag) continue;

		auto bagComp = bag->GetComponent<MoneyBagComponent>();
		if (!bagComp || !bagComp->IsFalling()) continue;

		auto bagBounds = bag->GetTransform()->GetWorldPosition();

		float overlapThreshold = 16.0f; // half tile width
		if (abs(nobbinBounds.x - bagBounds.x) < overlapThreshold &&
			abs(nobbinBounds.y - bagBounds.y) < overlapThreshold)
		{
			ChangeState(std::make_unique<GettingCrushedState>());
			return;
		}
	}

	auto* scene = dae::SceneManager::GetInstance().GetCurrentScene();
	if (!m_HasHitPlayer && scene)
	{
		for (auto& obj : scene->GetObjects())
		{
			if (!obj || obj.get() == GetOwner()) continue;

			auto lives = obj->GetComponent<dae::LivesComponent>();
			if (!lives) continue;

			auto playerPos = obj->GetTransform()->GetWorldPosition();

			float overlapThreshold = 16.0f;
			if (abs(nobbinBounds.x - playerPos.x) < overlapThreshold &&
				abs(nobbinBounds.y - playerPos.y) < overlapThreshold)
			{
				std::cout << "[Nobbin] Player hit!\n";
				lives->LoseLife();
				m_HasHitPlayer = true;
				break;
			}
		}
	}

	if (!m_pTracker)
		m_pTracker = GetOwner()->GetComponent<TileTrackerComponent>().get();

	if (m_pCurrentState)
		m_pCurrentState->Update(*this, deltaTime);

}

void dae::NobbinControllerComponent::ChangeState(std::unique_ptr<NobbinState> newState)
{
	if (m_pCurrentState && newState &&
		std::string(m_pCurrentState->GetID()) == std::string(newState->GetID()))
	{
		// Avoid redundant switch
		return;
	}

	if (m_pCurrentState)
	{
		m_pCurrentState->OnExit(*this);
	}

	m_pCurrentState = std::move(newState);

	if (m_pCurrentState)
	{
		m_pCurrentState->OnEnter(*this);
	}
}

void dae::NobbinControllerComponent::TryMoveInDirection(const glm::ivec2& direction)
{
	auto tracker = GetOwner()->GetComponent<TileTrackerComponent>();
	auto nobbinMover = GetOwner()->GetComponent<NobbinComponent>();
	if (!tracker || !nobbinMover || nobbinMover->NobbinIsMoving()) return;

	//std::cout << "[DEBUG] Attempting move while moving? " << nobbinMover->NobbinIsMoving() << "\n";

	auto currentTile = tracker->GetTileCoords();
	glm::ivec2 nextTile = currentTile + direction;

	glm::ivec2 dir = nextTile - currentTile;

	m_PreviousTile = currentTile;

	if (dir.x > 0) m_AnimState = NobbinAnimationState::WalkRight;
	else if (dir.x < 0) m_AnimState = NobbinAnimationState::WalkLeft;
	else if (dir.y > 0) m_AnimState = NobbinAnimationState::WalkDown;
	else if (dir.y < 0) m_AnimState = NobbinAnimationState::WalkUp;

	UpdateAnimationState();

	if (!IsDugTile(nextTile)) return;

	glm::vec3 worldPos = m_pLevelLoader->GetWorldCenterForTile(nextTile.x, nextTile.y);

	float distance = glm::distance(tracker->GetOwner()->GetTransform()->GetWorldPosition(), worldPos);
	float duration = distance / m_Speed;

	nobbinMover->NobbinStartMoveTo(worldPos, duration);
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

int dae::NobbinControllerComponent::GetTileCost(const glm::ivec2& tile) const
{
	auto playerTracker = m_pPlayer->GetComponent<TileTrackerComponent>();
	if (!playerTracker) return 999;

	const glm::ivec2 playerTile = playerTracker->GetTileCoords();
	int distanceCost = glm::abs(tile.x - playerTile.x) + glm::abs(tile.y - playerTile.y);

	// Add digging penalty
	int diggingPenalty = 0;
	if (!IsDugTile(tile)) diggingPenalty = 5; // cost of digging

	return distanceCost + diggingPenalty;
}

std::vector<glm::ivec2> dae::NobbinControllerComponent::FindPath(const glm::ivec2& start, const glm::ivec2& goal)
{
	std::vector<AStarNode> openList;
	std::vector<AStarNode> closedList;

	openList.push_back({ start,
						 0.0f,
						 static_cast<float>(glm::abs(start.x - goal.x) + glm::abs(start.y - goal.y)),
						 -1 });

	while (!openList.empty())
	{
		// 1) Pick lowest fCost in openList
		int bestIdx = 0;
		for (int i = 1; i < (int)openList.size(); ++i)
			if (openList[i].fCost() < openList[bestIdx].fCost())
				bestIdx = i;

		AStarNode current = openList[bestIdx];
		openList.erase(openList.begin() + bestIdx);

		// 2) Add to closed
		int closedIdx = (int)closedList.size();
		closedList.push_back(current);

		// 3) Check goal
		if (current.tile == goal)
		{
			// Reconstruct path
			std::vector<glm::ivec2> path;
			for (int idx = closedIdx; idx != -1; idx = closedList[idx].parentIndex)
				path.push_back(closedList[idx].tile);
			std::reverse(path.begin(), path.end());
			return path;
		}

		// 4) Explore neighbors
		static const std::array<glm::ivec2, 4> dirs = { {
			{1,0},{-1,0},{0,1},{0,-1}
		} };
		for (auto dir : dirs)
		{
			glm::ivec2 neighbor = current.tile + dir;

			if (!IsDugTile(neighbor)) continue;

			// Skip if in closedList
			bool inClosed = std::any_of(closedList.begin(), closedList.end(),
				[&](auto& n) { return n.tile == neighbor; });
			if (inClosed) continue;

			float tentativeG = current.gCost + 1.0f;
			float h = static_cast<float>( glm::abs(neighbor.x - goal.x) + glm::abs(neighbor.y - goal.y));

			// See if in openList
			auto it = std::find_if(openList.begin(), openList.end(),
				[&](auto& n) { return n.tile == neighbor; });

			if (it == openList.end())
			{
				openList.push_back({ neighbor, tentativeG, h, closedIdx });
			}
			else if (tentativeG < it->gCost)
			{
				it->gCost = tentativeG;
				it->parentIndex = closedIdx;
			}
		}
	}

	// No path
	return {};
}

void dae::NobbinControllerComponent::UpdateAnimationState()
{
	if (m_AnimState != m_LastAnimState)
	{
		if (auto animator = GetOwner()->GetComponent<SpriteAnimatorComponent>())
		{
			switch (m_AnimState)
			{
			case NobbinAnimationState::WalkRight:
				animator->PlayAnimation(0, 3); 
				break;
			case NobbinAnimationState::WalkDown:
				animator->PlayAnimation(3, 3); 
				break;
			case NobbinAnimationState::WalkLeft:
				animator->PlayAnimation(6, 3); 
				break;
			case NobbinAnimationState::WalkUp:
				animator->PlayAnimation(9, 3); 
				break;
			}
		}

		m_LastAnimState = m_AnimState;
	}
}
