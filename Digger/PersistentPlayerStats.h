#pragma once
namespace dae
{
	struct PersistentPlayerStats
	{
		int Lives = 3;
		int Score = 0;
	};

	inline PersistentPlayerStats g_Player1Stats;
	inline PersistentPlayerStats g_Player2Stats;
}