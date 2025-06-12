#pragma once
namespace dae
{
    class GameObject;

    //event id should not be engine level
    enum class EventId
    {
        PLAYER_DIED,
        PlAYER_HIT,
        PLAYER_GAINEDLIVES,
        PLAYER_ADDSCORE,
        PLAYER_REACHED_20000_POINTS
    };

    class Observer
    {
    public:
        virtual ~Observer() = default;
        virtual void Notify(EventId event, GameObject* gameObject) = 0;
    };
}