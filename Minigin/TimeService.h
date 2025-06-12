#pragma once

namespace dae 
{
    /// Holds the most-recent frame’s deltaTime so any code can grab it.
    class TimeService
    {
    public:
        static void SetDeltaTime(float dt) { s_DeltaTime = dt; }
        static float GetDeltaTime() noexcept { return s_DeltaTime; }

    private:
        static float s_DeltaTime;
    };
}
