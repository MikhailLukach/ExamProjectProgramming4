#include "ScoreboardUIControllerComponent.h"
#include "TextComponent.h"
#include "InputManager.h"
#include "GameObject.h"
#include <fstream>



dae::ScoreBoardUIControllerComponent::ScoreBoardUIControllerComponent()
{
	LoadScores("highscores.txt");
}

void dae::ScoreBoardUIControllerComponent::SetScoreTextObjects(const std::vector<std::shared_ptr<GameObject>>& scoreObjects)
{
	m_ScoreTextObjects.clear();
	for (const auto& obj : scoreObjects)
	{
		if (auto text = obj->GetComponent<TextComponent>())
			m_ScoreTextObjects.push_back(text);
	}
	UpdateScoreDisplay();
}

void dae::ScoreBoardUIControllerComponent::SetModeTextObjects(const std::vector<std::shared_ptr<GameObject>>& modeObjects)
{
	m_ModeTextObjects.clear();
	for (const auto& obj : modeObjects)
	{
		if (auto text = obj->GetComponent<TextComponent>())
			m_ModeTextObjects.push_back(text);
	}
	UpdateModeDisplay();
}

void dae::ScoreBoardUIControllerComponent::SetNewScore(int score)
{
	// Insert new score in first truly empty slot (--- 00000) or append
	auto it = std::find_if(m_HighScores.begin(), m_HighScores.end(),
		[](const HighScoreEntry& e) { return e.initials == "---" && e.score == 00000; });

	if (it != m_HighScores.end())
		*it = { "---", score };
	else
		m_HighScores.push_back({ "---", score });

	std::sort(m_HighScores.begin(), m_HighScores.end(), [](const auto& a, const auto& b) {
		return a.score > b.score;
		});

	// Trim to top 10
	if (m_HighScores.size() > 10)
		m_HighScores.pop_back();

	// Track entry for editing
	for (size_t i = 0; i < m_HighScores.size(); ++i)
	{
		if (m_HighScores[i].score == score && m_HighScores[i].initials == "---")
		{
			m_ScoreEntryIndicesToEdit.push_back(static_cast<int>(i));
			break;
		}
	}

	if (!m_EnteringInitials)
	{
		m_EnteringInitials = true;
		m_CurrentSection = Section::InitialsEntry;
		m_CurrentLetterIndex = 0;
		m_NumPlayersEntered = 0;
		m_NumPlayersToEnter = 2;
		m_ActiveScoreIndex = m_ScoreEntryIndicesToEdit[0];
	}

	UpdateScoreDisplay();
}

void dae::ScoreBoardUIControllerComponent::Update(float)
{
}

void dae::ScoreBoardUIControllerComponent::MoveCursorUp()
{
	if (m_CurrentSection != Section::GameModeSelection) return;

	if (m_SelectedModeIndex > 0)
		--m_SelectedModeIndex;

	UpdateModeDisplay();
}

void dae::ScoreBoardUIControllerComponent::MoveCursorDown()
{
	if (m_CurrentSection != Section::GameModeSelection) return;

	if (m_SelectedModeIndex < static_cast<int>(m_ModeTextObjects.size()) - 1)
		++m_SelectedModeIndex;

	UpdateModeDisplay();
}

void dae::ScoreBoardUIControllerComponent::IncrementLetter()
{
	if (!m_EnteringInitials || m_CurrentLetterIndex > 2) return;

	char& letter = m_InitialsBuffer[m_CurrentLetterIndex];
	letter = (letter == 'Z') ? 'A' : ((letter == '-') ? 'A' : letter + 1);

	UpdateScoreDisplay();
}

void dae::ScoreBoardUIControllerComponent::DecrementLetter()
{
	if (!m_EnteringInitials || m_CurrentLetterIndex > 2) return;

	char& letter = m_InitialsBuffer[m_CurrentLetterIndex];
	letter = (letter == 'A') ? 'Z' : ((letter == '-') ? 'Z' : letter - 1);

	UpdateScoreDisplay();
}

void dae::ScoreBoardUIControllerComponent::ConfirmSelection()
{
	if (m_EnteringInitials)
	{
		if (++m_CurrentLetterIndex >= 3)
		{
			// Finalize initials for this entry
			std::string finalInitials(m_InitialsBuffer, 3);
			m_HighScores[m_ActiveScoreIndex].initials = finalInitials;

			++m_NumPlayersEntered;
			if (m_NumPlayersEntered >= m_NumPlayersToEnter)
			{
				// All players done
				m_EnteringInitials = false;
				m_CurrentLetterIndex = 0;
				m_CurrentSection = Section::GameModeSelection;

				SaveScores("highscores.txt");
			}
			else
			{
				// Prepare next entry
				m_InitialsBuffer[0] = m_InitialsBuffer[1] = m_InitialsBuffer[2] = '-';
				m_CurrentLetterIndex = 0;

				std::cout << "[DEBUG] Player " << m_NumPlayersEntered
					<< " confirming initials for index: " << m_ActiveScoreIndex
					<< " score: " << m_HighScores[m_ActiveScoreIndex].score << "\n";

				// Insert new player entry just after the previous one
				m_ActiveScoreIndex = m_ScoreEntryIndicesToEdit[m_NumPlayersEntered];
				std::cout << "[DEBUG] Next active score index: " << m_ActiveScoreIndex
					<< " score: " << m_HighScores[m_ActiveScoreIndex].score << "\n";
			}

			UpdateScoreDisplay();
			UpdateModeDisplay();

		}
	}

	std::cout << "[DEBUG] Finished entering initials. Transitioning to GameModeSelection.\n";
}

void dae::ScoreBoardUIControllerComponent::MoveLetterLeft()
{
	if (m_CurrentSection != Section::InitialsEntry || m_CurrentLetterIndex <= 0) return;
	--m_CurrentLetterIndex;
	UpdateScoreDisplay();
}

void dae::ScoreBoardUIControllerComponent::MoveLetterRight()
{
	if (m_CurrentSection != Section::InitialsEntry || m_CurrentLetterIndex >= 2) return;
	++m_CurrentLetterIndex;
	UpdateScoreDisplay();
}

void dae::ScoreBoardUIControllerComponent::SaveScores(const std::string& filename) const
{
	std::ofstream outFile(filename);
	if (!outFile.is_open())
		return;

	for (const auto& entry : m_HighScores)
	{
		outFile << entry.initials << " " << entry.score << "\n";
	}
}

void dae::ScoreBoardUIControllerComponent::LoadScores(const std::string& filename)
{
	std::ifstream inFile(filename);
	if (!inFile.is_open())
		return;

	m_HighScores.clear();
	std::string initials;
	int score;

	while (inFile >> initials >> score)
	{
		m_HighScores.push_back({ initials, score });
	}

	// Ensure size is always 10
	while (m_HighScores.size() < 10)
		m_HighScores.push_back({ "---", 0 });

	UpdateScoreDisplay();
}

void dae::ScoreBoardUIControllerComponent::UpdateScoreDisplay()
{

	if (m_EnteringInitials)
		std::cout << "[DEBUG] Editing Letter Index: " << m_CurrentLetterIndex << "\n";

	for (size_t i = 0; i < m_ScoreTextObjects.size(); ++i)
	{
		const int score = (i < static_cast<int>(m_HighScores.size())) ? m_HighScores[i].score : 0;
		std::string initialsToDisplay = (i < static_cast<int>(m_HighScores.size())) ? m_HighScores[i].initials : "---";

		if (static_cast<int>(i) == m_ActiveScoreIndex && m_EnteringInitials)
		{
			std::cout << "[DEBUG] Drawing brackets around initials of index " << i
				<< " score: " << score << "\n";

			std::string formattedInitials;
			for (int j = 0; j < 3; ++j)
			{
				if (j == m_CurrentLetterIndex)
				{
					// Add brackets around the selected letter to show visual focus
					formattedInitials += '[';
					formattedInitials += m_InitialsBuffer[j];
					formattedInitials += ']';
				}
				else
				{
					formattedInitials += m_InitialsBuffer[j];
				}
			}
			initialsToDisplay = formattedInitials;
		}

		std::stringstream ss;
		ss << initialsToDisplay << " ";
		ss << std::setfill('0') << std::setw(5) << score;

		auto textComp = m_ScoreTextObjects[i];
		if (textComp)
		{
			std::string finalText = ss.str();
			textComp->SetText(finalText); 
			//std::cout << "[DEBUG] Updating score text: " << finalText << "\n";
		}
	}
}

void dae::ScoreBoardUIControllerComponent::UpdateModeDisplay()
{
	for (size_t i = 0; i < m_ModeTextObjects.size(); ++i)
	{
		std::string label = m_ModeTextObjects[i]->GetText();

		// Always remove any existing prefix
		if (label.starts_with("> "))
			label = label.substr(2);

		if (static_cast<int>(i) == m_SelectedModeIndex)
			m_ModeTextObjects[i]->SetText("> " + label);
		else
			m_ModeTextObjects[i]->SetText(label);
	}
}

