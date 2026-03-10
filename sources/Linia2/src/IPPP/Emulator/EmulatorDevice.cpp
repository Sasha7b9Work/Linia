// 2025/08/29 18:05:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/Emulator/EmulatorDevice.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"
#include "Communicator/UART/UART.h"


EmulatorDevice::~EmulatorDevice()
{
    Shutdown();
}


bool EmulatorDevice::Init()
{
    GPIO::Init();
//    SPI::Init();
    UART::Init(EmulatorDevice::CallbackOnReceive);

    LOG_ERROR("Not implemented");

    return false;
}


void EmulatorDevice::Update()
{

}


void EmulatorDevice::CallbackOnReceive(uint8 *, int)
{

}


void EmulatorDevice::EmulatorDevice::Shutdown()
{
    LOG_ERROR("Not implemented");
}


bool EmulatorDevice::IsConnected() const
{
    LOG_ERROR("Not implemented");

    return false;
}


void EmulatorDevice::SendCommand(pchar /*format*/, ...) const
{

}
