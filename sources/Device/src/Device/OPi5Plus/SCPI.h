// 2025/10/15 14:32:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


namespace OPi5Plus
{
    // Обмен по UART с Orange Pi 5 Plus

    namespace SCPI
    {
        bool Parse(pchar);

        void Send(pchar format, ...);
    }
}
