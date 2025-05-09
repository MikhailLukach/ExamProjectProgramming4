#include "MoneyBagComponent.h"
#include "MoneyBagState.h"
#include <iostream>

void dae::MoneyBagComponent::Update(float deltaTime)
{
	if(m_State)
	{
		auto newState = m_State->Update(*this, deltaTime);
		if (newState)
		{
			SetState(std::move(newState));
		}
	}
}

void dae::MoneyBagComponent::SetState(std::unique_ptr<MoneyBagState> newState)
{
	if (m_State)
	{
		m_State->OnExit(*this);
	}

	m_State = std::move(newState);

	if (m_State)
	{
		m_State->OnEnter(*this);
	}
}
