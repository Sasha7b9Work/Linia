// 2025/6/11 18:47:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/Comm.h"
#include "Communicator/ComPort/ComPort.h"
#include "Communicator/SPI/SPI.h"


void Comm::Init()
{
#ifdef WIN32

    ComPort::Update();

#else

    SPI::Init();

#endif
}

bool Comm::WriteDynamicDAC1(uint16_t value)
{
#ifdef WIN32
    return true;
#else
    return SPI::WriteDynamicDAC1(value);
#endif
}

bool Comm::WriteDynamicDAC2(uint16_t value)
{
#ifdef WIN32
    return true;
#else
    return SPI::WriteDynamicDAC2(value);
#endif
}

bool Comm::SetSPISpeed(uint32_t speedHz)
{
#ifdef WIN32
    return true;
#else
    return SPI::SetSpeed(speedHz);
#endif
}

bool Comm::SetSPIMode(uint8_t mode)
{
#ifdef WIN32
    return true;
#else
    return SPI::SetMode(mode);
#endif
}

bool Comm::IsSPIReady()
{
#ifdef WIN32
    return true;
#else
    return SPI::IsReady();
#endif
}
