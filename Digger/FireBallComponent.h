#pragma once
#include "Component.h"
#include <glm.hpp>

namespace dae 
{
    class TileManagerComponent;
    class ScoreComponent;

	class FireBallComponent : public Component
	{
    public:
        FireBallComponent(const glm::vec2& direction, float speed, float lifeSec, TileManagerComponent* tileManager, ScoreComponent* scoreComp);

        void Render() const override;

        void Update(float deltaTime) override;

    private:
        glm::vec2 m_Dir;
        float m_Speed;
        float m_RemainingLife;
        TileManagerComponent* m_pTileManager;
        ScoreComponent* m_pScore;
	};
}

