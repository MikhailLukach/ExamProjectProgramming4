#pragma once
namespace dae
{
    class GameObject;

    enum class EventId
    {
        PLAYER_DAMAGED,
        PLAYER_DIED,
        PLAYER_ADDSCORE,
        PLAYER_REACHED_500_POINTS
    };

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void Notify(EventId event, GameObject* gameObject) = 0;
    };
}