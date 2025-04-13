#pragma once
#include <iostream>
#include "GameObject.h"
//#include "HealthComponent.h"
//#include "ScoreComponent.h"

namespace dae
{
	class Command
	{
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}

