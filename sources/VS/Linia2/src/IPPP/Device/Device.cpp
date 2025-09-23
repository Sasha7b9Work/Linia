#include "defines.h"
#include "IPPP/Device/IDevice.h"
#include "IPPP/Device/Device.h"
#include "Communicator/UART/UART.h"


IDevice *IDevice::impl = nullptr;


Device::~Device()
{
    Shutdown();
}

bool Device::Init()
{
    UART::Init(Device::CallbackOnReceive);

    if(UART::Open())
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

    UART::Close();
    UART::DeInit();

    connected = false;
}


void Device::CallbackOnReceive(uint8)
{

}


bool Device::IsConnected() const
{
    return connected && UART::IsReady();
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
