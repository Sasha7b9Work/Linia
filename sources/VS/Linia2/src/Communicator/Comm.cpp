// 2025/6/11 18:47:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/Comm.h"
#include "Communicator/SPI/SPI.h"


void Comm::Init()
{
    SPI::Init();
}

bool Comm::WriteDynamicDAC1(uint16_t value)
{
    return SPI::WriteDynamicDAC1(value);
}

bool Comm::WriteDynamicDAC2(uint16_t value)
{
    return SPI::WriteDynamicDAC2(value);
}

bool Comm::SetSPISpeed(uint32_t speedHz)
{
    return SPI::SetSpeed(speedHz);
}

bool Comm::SetSPIMode(uint8_t mode)
{
    return SPI::SetMode(mode);
}

bool Comm::IsSPIReady()
{
    return SPI::IsReady();
}
