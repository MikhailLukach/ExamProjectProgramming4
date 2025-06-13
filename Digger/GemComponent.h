#pragma once
#include "Component.h"

namespace dae
{
	class GemComponent : public Component
	{
	public:
		void Render() const override;

		void Update(float deltaTime) override;
	private:
		static const std::array<std::string, 8> soundPaths;
	};
}

