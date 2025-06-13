#pragma once
#include <string>
#include <vector>
#include <algorithm>

namespace dae
{
    struct ScoreEntry
    {
        std::string initials;
        int score;
    };

    inline std::vector<ScoreEntry> g_HighScores(10);

    void AddNewScore(int score, const std::string& initials)
    {
        g_HighScores.push_back({ initials, score });
        std::sort(g_HighScores.begin(), g_HighScores.end(),
            [](const auto& a, const auto& b) { return a.score > b.score; });
        if (g_HighScores.size() > 10)
            g_HighScores.pop_back();
    }
}