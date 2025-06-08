#pragma once
#pragma once
#include "Component.h"

namespace dae
{
	class PlayerDebugComponent : public Component
	{
	public:
		void Render() const override;
	};
}

