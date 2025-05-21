#pragma once
#include "Component.h"

namespace dae
{
	class GemComponent : public Component
	{
		void Render() const override;

		void Update(float deltaTime) override;
	};
}

