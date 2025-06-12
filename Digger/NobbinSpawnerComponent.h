#pragma once
#include "Component.h"
#include "Observer.h"
#include <vector>
#include <memory>
#include <glm.hpp>

namespace dae
{
	class GameObject;
	class LevelLoader;
	class LevelManagerComponent;
    class TileManagerComponent;
	class Scene;

	class NobbinSpawnerComponent : public Component, public Observer
	{
    public:
        NobbinSpawnerComponent(Scene* scene, LevelManagerComponent* levelMgr, LevelLoader* loader, TileManagerComponent* tileManager,
            int tileX, int tileY, float spawnDelay = 5.f, int maxNobbins = 3);

        void Update(float deltaTime) override;
        //void OnNobbinDestroyed(GameObject* nobbin);
        void Notify(EventId event, GameObject* gameObject) override;
    private:
        void SpawnNobbin();
        void KillAllNobbins();

        Scene* m_Scene{};
        LevelManagerComponent* m_LevelManager{};
        LevelLoader* m_LevelLoader{};
        TileManagerComponent* m_TileManager{};
        glm::ivec2 m_SpawnTile{};
        float m_SpawnDelay{};
        float m_Timer{};
        int m_MaxNobbins{};
        bool m_IsPaused{ false };
        float m_PauseTimer{ 0.f };

        std::vector<std::weak_ptr<GameObject>> m_LiveNobbins;
	};
}

