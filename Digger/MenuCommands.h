#pragma once
#include "Command.h"
#include "ScoreboardUIControllerComponent.h"

namespace dae
{
	class ScoreBoardUIControllerComponent; // Forward declaration

	class MoveLetterLeftCommand final : public Command
	{
	public:
		explicit MoveLetterLeftCommand(dae::ScoreBoardUIControllerComponent* pComp) : m_pComponent(pComp) {}
		void Execute() override { if (m_pComponent) m_pComponent->MoveLetterLeft(); }
	private:
		dae::ScoreBoardUIControllerComponent* m_pComponent;
	};

	class MoveLetterRightCommand final : public Command
	{
	public:
		explicit MoveLetterRightCommand(dae::ScoreBoardUIControllerComponent* pComp) : m_pComponent(pComp) {}
		void Execute() override { if (m_pComponent) m_pComponent->MoveLetterRight(); }
	private:
		dae::ScoreBoardUIControllerComponent* m_pComponent;
	};

	class VerticalMenuCommand : public dae::Command
	{
	public:
		VerticalMenuCommand(dae::ScoreBoardUIControllerComponent* controller, bool up)
			: m_Controller(controller), m_Up(up) {
		}

		void Execute() override
		{
			if (!m_Controller)
				return;

			if (m_Controller->GetCurrentSection() == dae::Section::InitialsEntry)
			{
				if (m_Up)
					m_Controller->IncrementLetter();
				else
					m_Controller->DecrementLetter();
			}
			else if (m_Controller->GetCurrentSection() == dae::Section::GameModeSelection)
			{
				if (m_Up)
					m_Controller->MoveCursorUp();
				else
					m_Controller->MoveCursorDown();
			}
		}

	private:
		dae::ScoreBoardUIControllerComponent* m_Controller;
		bool m_Up;
	};

	class ConfirmSelectionCommand final : public Command
	{
	public:
		explicit ConfirmSelectionCommand(ScoreBoardUIControllerComponent* controller)
			: m_pController(controller) {
		}

		void Execute() override
		{
			if (!m_pController) return;

			switch (m_pController->GetCurrentSection())
			{
			case Section::InitialsEntry:
				m_pController->ConfirmSelection(); // Moves to next letter or ends input
				break;
			case Section::GameModeSelection:
				// Maybe call StartGame(m_pController->GetSelectedMode()) or similar later
				m_pController->ConfirmSelection();
				break;
			}
		}

	private:
		ScoreBoardUIControllerComponent* m_pController;
	};
}
