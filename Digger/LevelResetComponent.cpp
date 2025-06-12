#include "LevelResetComponent.h"
#include "SceneManager.h"
#include "Scene.h"



dae::LevelResetComponent::LevelResetComponent(std::function<void()> reloadFunc, bool isCoop)
    : m_ReloadFunc(reloadFunc)
    , m_DeathThreshold(isCoop ? 2 : 1)
{
}

void dae::LevelResetComponent::Update(float deltaTime)
{
    if (!m_Waiting) return;

    m_Timer -= deltaTime;
    if (m_Timer > 0.0f) return;

    m_Waiting = false;

    // clear and reload immediately
    SceneManager::GetInstance().RequestReload(m_ReloadFunc);
}

void dae::LevelResetComponent::Notify(EventId event, GameObject*)
{
    if (event != EventId::PLAYER_DIED || m_Waiting)
    {
        return;
    }

    ++m_DeathsSeen;
    std::cout << "[LevelResetComponent] Deaths seen increased to: " << m_DeathsSeen << " required deaths: " << m_DeathThreshold << std::endl;
    if (m_DeathsSeen >= m_DeathThreshold)
    {
        m_Waiting = true;
        m_Timer = kDelay;
    }
}
