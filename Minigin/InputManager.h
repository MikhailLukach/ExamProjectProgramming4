#pragma once
#include "Singleton.h"
#include "GameController.h"
#include "Command.h"
#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <unordered_set>

namespace dae
{
	enum class InputType
	{
		Pressed,  
		Released, 
		Down      
	};
	class InputManager final : public Singleton<InputManager>
	{
	public:
		InputManager();
		~InputManager() = default;

		bool ProcessInput(); // Handles keyboard & gamepad input

		// Controller Support
		void AddController(int controllerIndex);
		void RemoveController(int controllerIndex);
		bool IsControllerConnected(int controllerIndex) const;

		// Command Binding
		void BindCommandController(int controllerIndex, unsigned int button, InputType state, std::unique_ptr<Command> command);
		void BindCommandKeyboard(SDL_Keycode key, InputType state, std::unique_ptr<Command> command);

		//enable/disable input
		void DisableInputFor(dae::GameObject* obj);
		void EnableInputFor(dae::GameObject* obj);
		bool IsInputEnabled(dae::GameObject* obj) const;
	private:
		//std::vector<GameController> m_Controllers;
		std::vector<std::unique_ptr<GameController>> m_Controllers;
		std::unordered_map<int, std::unordered_map<unsigned int, std::unordered_map<InputType, std::unique_ptr<Command>>>> m_ControllerCommands;

		std::unordered_map<SDL_Keycode, std::unordered_map<InputType, std::unique_ptr<Command>>> m_KeyboardCommands;

		std::unordered_map<SDL_Keycode, bool> m_PreviousKeyStates;

		std::unordered_set<dae::GameObject*> m_DisabledObjects;

		glm::ivec2 m_CurrentDirection{ 0, 0 };
	};
}
