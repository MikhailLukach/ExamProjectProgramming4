#include "NobbinControllerComponent.h"
#include "TileManagerComponent.h"
#include "TileTrackerComponent.h"
#include "SpriteAnimatorComponent.h"
#include "NobbinComponent.h"
#include "TileComponent.h"
#include "AIUtils.h"
#include "TileTypes.h"
#include "LevelLoader.h"
#include "NobbinState.h"
#include "ChasingState.h"
#include "ChasingAndDiggingState.h"

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
	ChangeState(std::make_unique<ChasingState>());
}

void dae::NobbinControllerComponent::Update(float deltaTime)
{
	(void)deltaTime;

	if (!m_pTracker)
		m_pTracker = GetOwner()->GetComponent<TileTrackerComponent>().get();

	if (m_pCurrentState)
		m_pCurrentState->Update(*this, deltaTime);

}

void dae::NobbinControllerComponent::ChangeState(std::unique_ptr<NobbinState> newState)
{
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

	m_PreviousTile = currentTile;

	//m_PostMoveTimer = m_PostMoveDelay;
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

void dae::NobbinControllerComponent::UpdateAnimationState()
{
	if (m_AnimState != m_LastAnimState)
	{
		if (auto animator = GetOwner()->GetComponent<SpriteAnimatorComponent>())
		{
			switch (m_AnimState)
			{
			case NobbinAnimationState::WalkRight:
				std::cout << "[NobbinControllerComponent] actually updating animstate." << std::endl;
				animator->PlayAnimation(0, 3); 
				break;
			case NobbinAnimationState::WalkDown:
				std::cout << "[NobbinControllerComponent] actually updating animstate." << std::endl;
				animator->PlayAnimation(3, 3); 
				break;
			case NobbinAnimationState::WalkLeft:
				std::cout << "[NobbinControllerComponent] actually updating animstate." << std::endl;
				animator->PlayAnimation(6, 3); 
				break;
			case NobbinAnimationState::WalkUp:
				std::cout << "[NobbinControllerComponent] actually updating animstate." << std::endl;
				animator->PlayAnimation(9, 3); 
				break;
			}
		}

		m_LastAnimState = m_AnimState;
	}
}
