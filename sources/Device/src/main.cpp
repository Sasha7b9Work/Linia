// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Device/OPi5Plus/OPi5Plus.h"
#include "Device/Device.h"
#include "Hardware/VCP.h"


int main()
{
    HAL::Init();

    HAL_TIM::Delay(500);

    Device::Init();

    while (true)
    {
        OPi5Plus::PeriodicTask();

        VCP::PeriodicTask();

        Device::PeriodicTask();
    }
}
