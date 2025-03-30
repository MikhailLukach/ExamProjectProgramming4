#pragma once
namespace dae
{
    class GameObject;

    //event id should not be engine level
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