#pragma once
#include "Component.h"
#include "TextComponent.h"
#include <memory>

namespace dae
{
	class FPSComponent : public Component
	{
	public:
		explicit FPSComponent(std::shared_ptr<TextComponent> textComponent);
		void Update(float deltaTime) override;
	private:
		std::weak_ptr<TextComponent> m_pTextComponent;
		float m_ElapsedTime = 0.0f;
		int m_FrameCount = 0;
	};
}

