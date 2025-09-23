// 2025/08/29 18:05:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Device/DeviceEmulator.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"
#include "Communicator/UART/UART.h"


bool DeviceEmulator::Init()
{
    GPIO::Init();
    SPI::Init();
    UART::Init();

    LOG_ERROR("Not implemented");

    return false;
}


void DeviceEmulator::DeviceEmulator::Shutdown()
{
    LOG_ERROR("Not implemented");
}


bool DeviceEmulator::IsConnected() const
{
    LOG_ERROR("Not implemented");

    return false;
}


void DeviceEmulator::WriteMicroChip(MicroChip::E, int, uint)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetFirstQueue(const Chan &)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetPulseDuration(uint /*durationUS*/, bool)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::StartMeasurement()
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::StopMeasurement()
{
    LOG_ERROR("Not implemented");
}
