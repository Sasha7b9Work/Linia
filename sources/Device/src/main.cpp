// (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
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

    LOG_WRITE("Start STM32");

    while (true)
    {
        OPi5Plus::PeriodicTask();

        VCP::PeriodicTask();

        Device::PeriodicTask();

        if (HAL_USART1::error)
        {
            LOG_ERROR("USART error %d", HAL_USART1::error);

            HAL_USART1::error = 0;
        }
    }
}
