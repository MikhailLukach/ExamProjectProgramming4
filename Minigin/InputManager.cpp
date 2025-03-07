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

	//change -> to .
	for (auto& controller : m_Controllers)
	{
		controller->Update(); 

		for (const auto& [button, commandMap] : m_ControllerCommands[controller->GetControllerIndex()])
		{
			if (controller->IsDownThisFrame(button) && commandMap.count(InputType::Down))
			{
				commandMap.at(InputType::Down)->Execute();
			}
			else if (controller->IsPressed(button) && commandMap.count(InputType::Pressed))
			{
				commandMap.at(InputType::Pressed)->Execute();
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