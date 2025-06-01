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

    // Теймер "сработает" через time ms (Значение IsFinished() будет true)
    void SetResponseTime(uint time);

    // Вернёт true, когда закончится промежуток time из функции SetResponseTime
    bool IsFinished() const;

    void WaitFor(uint timeMS);

private:

    uint time_reset;        // От этого времени отсчитывается ElapsedTime()

    uint time_response;     // После этого времени нужно вернуть true в IsFinished()
};
