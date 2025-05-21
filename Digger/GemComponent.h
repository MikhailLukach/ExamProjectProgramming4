#pragma once
#include "Component.h"

namespace dae
{
	class GemComponent : public Component
	{
		void Update(float deltaTime) override;
	};
}

