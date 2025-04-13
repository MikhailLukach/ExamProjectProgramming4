#include "SpriteAnimatorComponent.h"
#include "RenderComponent.h"
#include "Texture2D.h"
#include <iostream>

dae::SpriteAnimatorComponent::SpriteAnimatorComponent(RenderComponent* renderComp, int frameWidth, int frameHeight, float frameDuration)
	: m_pRenderComponent(renderComp)
	,m_FrameWidth(frameWidth)
	,m_FrameHeight(frameHeight)
	,m_FrameDuration(frameDuration)
{
	if (!m_pRenderComponent)
	{
		std::cerr << "SpriteAnimatorComponent requires a valid RenderComponent!\n";
	}
}

void dae::SpriteAnimatorComponent::Update(float deltaTime)
{
	if (!m_IsPlaying || !m_pRenderComponent) return;

	m_FrameTimer += deltaTime;
	if (m_FrameTimer >= m_FrameDuration)
	{
		m_FrameTimer -= m_FrameDuration;
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_FrameCount;
		UpdateTextureClip();
	}
}

void dae::SpriteAnimatorComponent::PlayAnimation(int startFrame, int frameCount)
{
	if (m_IsPlaying &&
		m_StartFrame == startFrame &&
		m_FrameCount == frameCount)
	{
		return;
	}

	//std::cout << "Switching animation!" << std::endl;

	m_StartFrame = startFrame;
	m_FrameCount = frameCount;
	m_CurrentFrameIndex = 0;
	m_FrameTimer = 0.f;
	m_IsPlaying = true;

	UpdateTextureClip();
}

void dae::SpriteAnimatorComponent::Stop()
{
	m_IsPlaying = false;
}

void dae::SpriteAnimatorComponent::Play(AnimationState state)
{
	if (m_CurrentState != state)
	{
		m_CurrentState = state;
		m_CurrentFrameIndex = 0;
		m_FrameTimer = 0.f;
	}

	m_IsPlaying = true;
	UpdateTextureClip();
}

void dae::SpriteAnimatorComponent::UpdateTextureClip()
{
	SDL_Rect clip{};
	clip.x = (m_StartFrame + m_CurrentFrameIndex) * m_FrameWidth;
	clip.y = 0;
	clip.w = m_FrameWidth;
	clip.h = m_FrameHeight;

	m_pRenderComponent->SetSourceRect(clip);
}
