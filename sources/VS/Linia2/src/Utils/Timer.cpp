// 2022/05/24 15:13:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Timer.h"


void TimerMS::Reset()
{
    time_reset = (uint)std::clock();
}


uint TimerMS::ElapsedTime()
{
    return std::clock() - time_reset;
}


void TimeMeterMS::Reset()
{
    time_reset = std::clock();
    time_response = time_reset;
}


int64 Timer::CurrentTimeMS()
{
    std::chrono::milliseconds duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

    return duration.count();
}


uint Timer::CurrentTimeSec()
{
    return (uint)(CurrentTimeMS() / 1000);
}


void Timer::PauseOnMS(uint timeMS)
{
    int64 time_end = CurrentTimeMS() + timeMS;

    while (CurrentTimeMS() < time_end)
    {
    }
}


float TimeMeterMS::ElapsedMS() const
{
    return (1000.0f * (float)(std::clock() - time_reset)) / CLOCKS_PER_SEC;
}
