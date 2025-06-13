#pragma once
#include "Component.h"
#include <vector>
#include <memory>
#include <string>
#include <functional>

namespace dae
{
	class TextComponent;

	struct HighScoreEntry {
		std::string initials = "---";
		int score = 0;
		int playerIndex = 0;
	};

	enum class Section
	{
		GameModeSelection,
		InitialsEntry
	};

	class ScoreBoardUIControllerComponent final : public Component
	{
	public:
		ScoreBoardUIControllerComponent();
		void SetScoreTextObjects(const std::vector<std::shared_ptr<GameObject>>& scoreObjects);
		void SetModeTextObjects(const std::vector<std::shared_ptr<GameObject>>& modeObjects);
		void SetNewScore(int score);

		void SetNumPlayersToEnter(int num);

		void Update(float deltaTime) override;

		void MoveCursorUp();
		void MoveCursorDown();
		void IncrementLetter();
		void DecrementLetter();
		void ConfirmSelection();

		void MoveLetterLeft();
		void MoveLetterRight();

		void SaveScores(const std::string& filename) const;
		void LoadScores(const std::string& filename);

		Section GetCurrentSection() const { return m_CurrentSection; }

		void SetLoadModeCallback(std::function<void(int)> callback);
	private:
		void UpdateScoreDisplay();
		void UpdateModeDisplay();

		std::vector<HighScoreEntry> m_HighScores{ 10 };
		std::vector<std::shared_ptr<TextComponent>> m_ScoreTextObjects;
		std::vector<std::shared_ptr<TextComponent>> m_ModeTextObjects;

		int m_ActiveScoreIndex{ -1 };     // Highscore index being edited
		int m_CurrentLetterIndex{ 0 };    // Which initial letter is active
		bool m_EnteringInitials{ false }; // Are we editing initials
		int m_SelectedModeIndex{ 0 };     // Game mode index
		char m_InitialsBuffer[3]{ '-', '-', '-' }; // Temp initials

		Section m_CurrentSection{ Section::GameModeSelection };

		std::vector<std::string> m_ModeNames = { "Single Player", "Versus Mode", "Coop Mode" };
		std::vector<int> m_ScoreEntryIndicesToEdit;

		int m_NumPlayersToEnter{ 1 };      // Set to 2 for versus/coop
		int m_NumPlayersEntered{ 0 };

		std::function<void(int)> m_LoadModeCallback;
	};

}
