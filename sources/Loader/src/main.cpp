// 2026/09/22 10:10:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Device/Device.h"
#include "Upgrader.h"
#include "Device/OPi5Plus/OPi5Plus.h"
#include <stm32f4xx_hal.h>


int main()
{
    SCB->VTOR = 0x080C0000;
    
    __enable_irq();
    
    HAL::Init();

    HAL_TIM::Delay(500);

    Device::Init();

    Upgrader::BeginUpgrade();

    while (true)
    {
        OPi5Plus::PeriodicTask();

        Device::PeriodicTask();
    }
}
