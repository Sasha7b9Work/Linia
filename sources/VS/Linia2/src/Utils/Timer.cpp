// 2022/05/24 15:13:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Timer.h"
#include <ctime>
#include <chrono>


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
    time_reset = (uint)std::clock();
    time_response = time_reset;
}


void TimeMeterMS::SetResponseTime(uint time)
{
    time_response = (uint)std::clock() + time;
}


bool TimeMeterMS::IsFinished() const
{
    return ((uint)std::clock() >= time_response);
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


void TimeMeterMS::WaitFor(uint timeMS)
{
    SetResponseTime(timeMS);

    while (!IsFinished())
    {
    }
}
