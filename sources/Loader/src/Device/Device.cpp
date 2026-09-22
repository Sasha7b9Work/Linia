// 2025/11/27 15:47:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Device/Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"


void Device::Init()
{
    Timer::Init();
}


void Device::PeriodicTask()
{
    HAL_TIM2::StartMultiMeasurement();
}
