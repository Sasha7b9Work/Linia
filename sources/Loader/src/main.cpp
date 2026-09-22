// 2026/09/22 10:10:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Device/OPi5Plus/OPi5Plus.h"
#include "Device/Device.h"
#include "Device/OPi5Plus/SCPI.h"


int main()
{
    HAL::Init();

    HAL_TIM::Delay(500);

    Device::Init();

    // Команда начала обновления уже получена, стираем сектор, где будет храниться прошивка
    HAL_FLASH::Firmware::EraseSector();

    OPi5Plus::SCPI::Send(":UPGRADE:START 1");

    while (true)
    {
        OPi5Plus::PeriodicTask();

        Device::PeriodicTask();
    }
}
