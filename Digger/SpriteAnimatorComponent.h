#pragma once
#include "Component.h"
#include "AnimationState.h"
#include <unordered_map>
#include <vector>
#include <SDL_rect.h>
#include <string>

namespace dae
{
	class RenderComponent;

	class SpriteAnimatorComponent final : public Component
	{
	public:
		SpriteAnimatorComponent(RenderComponent* renderComp, int frameWidth, int frameHeight, float frameDuration);

		void Update(float deltaTime) override;
		void PlayAnimation(int startFrame, int frameCount, bool loop = true);
		void Stop();

		void Play(AnimationState state);

		void Configure(RenderComponent* render, int frameWidth, int frameHeight, float frameDuration);

		bool IsPlaying() const { return m_IsPlaying; }
	private:
		RenderComponent* m_pRenderComponent;
		std::vector<SDL_Rect> m_Frames;

		int m_CurrentFrameIndex{};
		int m_StartFrame{};
		int m_FrameCount{};

		float m_FrameTimer{};
		float m_FrameDuration;

		bool m_IsPlaying{ false };
		bool m_Looping{ true };

		int m_FrameWidth;
		int m_FrameHeight;

		AnimationState m_CurrentState{ AnimationState::WalkDown };

		std::unordered_map<AnimationState, int> m_AnimationRowMapping;

		void UpdateTextureClip();
	};
}

