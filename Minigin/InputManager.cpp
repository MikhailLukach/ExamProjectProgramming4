#include <SDL.h>
#include "InputManager.h"
#include "imgui.h"
#include <iostream>
#include <backends/imgui_impl_sdl2.h>
#include <backends/imgui_impl_opengl3.h>
#include <algorithm>

dae::InputManager::InputManager()
{
	//m_Controllers.emplace_back(0);
	m_Controllers.emplace_back(std::make_unique<GameController>(0));
}

bool dae::InputManager::ProcessInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
		{
			bool isPressed = (e.type == SDL_KEYDOWN);
			for (const auto& [key, commandMap] : m_KeyboardCommands)
			{
				if (e.key.keysym.sym == key)
				{
					if (isPressed && commandMap.count(InputType::Down))
					{
						commandMap.at(InputType::Down)->Execute();
					}
					else if (!isPressed && commandMap.count(InputType::Released))
					{
						commandMap.at(InputType::Released)->Execute();
					}
				}
			}
		}
		ImGui_ImplSDL2_ProcessEvent(&e); 
	}

	const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
	for (const auto& [key, commandMap] : m_KeyboardCommands)
	{
		bool isDown = keyboardState[SDL_GetScancodeFromKey(key)];

		if (isDown)
		{
			if (m_PreviousKeyStates[key] && commandMap.count(InputType::Pressed)) 
			{
				commandMap.at(InputType::Pressed)->Execute();
			}
		}
		else if (m_PreviousKeyStates[key] && commandMap.count(InputType::Released)) 
		{
			commandMap.at(InputType::Released)->Execute();
		}

		m_PreviousKeyStates[key] = isDown;
	}

	m_CurrentDirection = { 0, 0 };

	//change -> to .
	for (auto& controller : m_Controllers)
	{
		controller->Update(); 

		bool up = controller->IsPressed(GameController::DPAD_UP);
		bool down = controller->IsPressed(GameController::DPAD_DOWN);
		bool left = controller->IsPressed(GameController::DPAD_LEFT);
		bool right = controller->IsPressed(GameController::DPAD_RIGHT);

		// Compute intended direction (clamped to one axis)
		if (up && !down)       m_CurrentDirection.y = -1;
		else if (down && !up)  m_CurrentDirection.y = 1;

		if (left && !right)    m_CurrentDirection.x = -1;
		else if (right && !left) m_CurrentDirection.x = 1;

		// Clamp to prevent diagonal movement (optional: prioritize vertical)
		if (m_CurrentDirection.x != 0 && m_CurrentDirection.y != 0)
		{
			// Comment one of these to prioritize the other
			m_CurrentDirection.y = 0; // prioritizes horizontal over vertical
			// m_CurrentDirection.x = 0; // prioritizes vertical over horizontal
		}

		// Now dispatch only one direction based on m_CurrentDirection
		if (m_CurrentDirection.x == -1)
		{
			auto& map = m_ControllerCommands[controller->GetControllerIndex()][GameController::DPAD_LEFT];
			if (map.count(InputType::Pressed)) map.at(InputType::Pressed)->Execute();
		}
		else if (m_CurrentDirection.x == 1)
		{
			auto& map = m_ControllerCommands[controller->GetControllerIndex()][GameController::DPAD_RIGHT];
			if (map.count(InputType::Pressed)) map.at(InputType::Pressed)->Execute();
		}
		else if (m_CurrentDirection.y == -1)
		{
			auto& map = m_ControllerCommands[controller->GetControllerIndex()][GameController::DPAD_UP];
			if (map.count(InputType::Pressed)) map.at(InputType::Pressed)->Execute();
		}
		else if (m_CurrentDirection.y == 1)
		{
			auto& map = m_ControllerCommands[controller->GetControllerIndex()][GameController::DPAD_DOWN];
			if (map.count(InputType::Pressed)) map.at(InputType::Pressed)->Execute();
		}

		//std::cout << "Clamped Direction: (" << m_CurrentDirection.x << ", " << m_CurrentDirection.y << ")\n";
		for (const auto& [button, commandMap] : m_ControllerCommands[controller->GetControllerIndex()])
		{
			if (controller->IsDownThisFrame(button) && commandMap.count(InputType::Down))
			{
				commandMap.at(InputType::Down)->Execute();
			}
			else if (controller->IsUpThisFrame(button) && commandMap.count(InputType::Released))
			{
				commandMap.at(InputType::Released)->Execute();
			}
		}
	}

	return true;
}

void dae::InputManager::AddController(int controllerIndex)
{
	if (!IsControllerConnected(controllerIndex))
	{
		//m_Controllers.emplace_back(controllerIndex);
		m_Controllers.emplace_back(std::make_unique<GameController>(controllerIndex));
	}
}

void dae::InputManager::RemoveController(int controllerIndex)
{
	/*m_Controllers.erase
	(
		std::remove_if(m_Controllers.begin(), m_Controllers.end(),
		[controllerIndex](const GameController& controller) { return controller.GetControllerIndex() == controllerIndex; }),
		m_Controllers.end()
	);*/
	m_Controllers.erase(
		std::remove_if(m_Controllers.begin(), m_Controllers.end(),
			[controllerIndex](const std::unique_ptr<GameController>& controller) { return controller->GetControllerIndex() == controllerIndex; }),
		m_Controllers.end()
	);
}

bool dae::InputManager::IsControllerConnected(int controllerIndex) const
{
	/*return std::any_of(m_Controllers.begin(), m_Controllers.end(), [controllerIndex](const GameController& controller)
		{ return controller.GetControllerIndex() == controllerIndex; });*/
	return std::any_of(m_Controllers.begin(), m_Controllers.end(), [controllerIndex](const std::unique_ptr<GameController>& controller)
		{ return controller->GetControllerIndex() == controllerIndex; });
}

void dae::InputManager::BindCommandController(int controllerIndex, unsigned int button, InputType state, std::unique_ptr<Command> command)
{
	m_ControllerCommands[controllerIndex][button][state] = std::move(command);
}

void dae::InputManager::BindCommandKeyboard(SDL_Keycode key, InputType state, std::unique_ptr<Command> command)
{
	m_KeyboardCommands[key][state] = std::move(command);
}

void dae::InputManager::DisableInputFor(dae::GameObject* obj)
{
	m_DisabledObjects.insert(obj);
}

void dae::InputManager::EnableInputFor(dae::GameObject* obj)
{
	m_DisabledObjects.erase(obj);
}

bool dae::InputManager::IsInputEnabled(dae::GameObject* obj) const
{
	return m_DisabledObjects.find(obj) == m_DisabledObjects.end();
}
