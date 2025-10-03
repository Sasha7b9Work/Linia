#include "defines.h"
#include "IPPP/Device/IDevice.h"
#include "IPPP/Device/Device.h"
#include "Communicator/UART/UART.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"

IDevice *IDevice::impl = nullptr;


Device::~Device()
{
    Shutdown();
}

bool Device::Init()
{
    GPIO::Init();
    SPI::Init();

    if(UART::Init(Device::CallbackOnReceive))
    {
        connected = true;
        running = true;
        return true;
    }

    return false;
}


void Device::Shutdown()
{
    running = false;

    UART::DeInit();

    connected = false;
}


void Device::CallbackOnReceive(uint8)
{

}


bool Device::IsConnected() const
{
    return connected;
}


void Device::SendCommand(pchar format, ...)
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    std::strcat(message, "\r");

    UART::SendBuffer(message, (int)std::strlen(message));
}


void Device::WriteMicroChip(MicroChip::E chip, int reg, uint value)
{
    SendCommand(":CHIP:%s:%d:WRITE %08X", MicroChip::Name(chip), reg, value);
}
