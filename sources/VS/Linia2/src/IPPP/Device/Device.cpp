#include "defines.h"
#include "IPPP/Device/IDevice.h"
#include "IPPP/Device/Device.h"
#include "Communicator/UART/UART.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"
#include "IPPP/Device/Chips.h"

IDevice *IDevice::impl = nullptr;


Device::~Device()
{
    Shutdown();
}

bool Device::Init()
{
    GPIO::Init();
    SPI::Init();

    Chip::Init();

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


void Device::CallbackOnReceive(uint8 /*byte*/)
{
//    LOG_WRITE("receive %c", (char)byte);
}


bool Device::IsConnected() const
{
    return connected;
}


void Device::SendCommand(pchar format, ...) const
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    std::strcat(message, "\0");

    UART::SendBuffer(message, (int)std::strlen(message) + 1);
}
