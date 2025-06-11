/*#pragma once
#include "Component.h"
#include "Subject.h"

namespace dae
{
    class HealthComponent : public Component, public Subject
    {
    public:
        explicit HealthComponent(int lives);
        void TakeDamage(int damage);
        int GetLives() const;

    private:
        int m_Lives;
    };
}*/