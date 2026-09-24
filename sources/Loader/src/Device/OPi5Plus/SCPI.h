// 2025/10/15 14:32:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once


namespace OPi5Plus
{
    // Обмен по UART с Orange Pi 5 Plus

    namespace SCPI
    {
        // Сюда подаётся строка, заканчивающаяся нулём
        void Parse(pchar);

        void Send(pchar format, ...);
    }
}
