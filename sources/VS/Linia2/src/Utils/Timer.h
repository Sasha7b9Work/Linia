// 2022/05/24 15:12:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Timer
{
    int64 CurrentTimeMS();
    uint CurrentTimeSec();

    void PauseOnMS(uint);
}


class TimerMS
{
public:

    TimerMS() { Reset(); }

    void Reset();

    uint ElapsedTime();

private:

    uint time_reset = 0;
};



// Структура для отсчёта времени
struct TimeMeterMS
{
    TimeMeterMS()
    {
        Reset();
    }

    // Установить момент отсчёта
    void Reset();

    float ElapsedMS() const;

private:

    clock_t time_reset;        // От этого времени отсчитывается ElapsedTime()

    clock_t time_response;     // После этого времени нужно вернуть true в IsFinished()
};


struct TimeMeterNS
{
    TimeMeterNS()
    {
        Reset();
    }

    void Reset();

    float ElapsedNS() const;

private:

    std::chrono::high_resolution_clock::time_point time_reset;
};