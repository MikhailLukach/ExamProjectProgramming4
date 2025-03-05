#include "GameController.h"

dae::GameController::GameController(int controllerIndex)
	: m_ControllerIndex(controllerIndex), m_Connected(false)
{
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_PreviousState, sizeof(XINPUT_STATE));
}

void dae::GameController::Update()
{
	m_PreviousState = m_CurrentState;
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));

	if (XInputGetState(m_ControllerIndex, &m_CurrentState) == ERROR_SUCCESS)
	{
		m_Connected = true;

		unsigned int previousButtons = m_PreviousState.Gamepad.wButtons;
		unsigned int currentButtons = m_CurrentState.Gamepad.wButtons;

		buttonsPressedThisFrame = (currentButtons) & (~previousButtons);
		buttonsReleasedThisFrame = (~currentButtons) & (previousButtons);
	}
	else
	{
		m_Connected = false;
		buttonsPressedThisFrame = 0;
		buttonsReleasedThisFrame = 0;
	}
}

bool dae::GameController::IsDownThisFrame(unsigned int button) const
{
	return buttonsPressedThisFrame & button;
}

bool dae::GameController::IsUpThisFrame(unsigned int button) const
{
	return buttonsReleasedThisFrame & button;
}

bool dae::GameController::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

