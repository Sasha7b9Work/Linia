// 2024/7/18 14:34:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Reader/Indicator.h"
#include "Reader/Reader.h"
#include "Utils/Timer.h"


void Indicator::On()
{
    Reader::Send("#LED ON FFFF0000");
}


void Indicator::Off()
{
    Reader::Send("#LED ON 00000000");
}


void Indicator::Card()
{
    Reader::Send("#LED ON FF00FF00");
}


void Indicator::Task()
{
    static bool is_fire = false;

    static int64 next_time = 0;

    if (Timer::CurrentTimeMS() < next_time)
    {
        return;
    }

    next_time = Timer::CurrentTimeMS() + 10;

    is_fire = !is_fire;

    is_fire ? Reader::Send("#LED ON FFFFFFFF") : Reader::Send("#LED ON FF0000FF");
}
