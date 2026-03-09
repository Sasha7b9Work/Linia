#include "defines.h"
#include "IPPP/Device/IDevice.h"
#include "IPPP/Device/Device.h"
#include "Communicator/UART/UART.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"
#include "IPPP/Device/Chips.h"
#include "IPPP/SCPI/SCPI.h"
#include "Communicator/ComPort/ComPort.h"

IDevice *IDevice::impl = nullptr;


Device::~Device()
{
    Shutdown();
}

bool Device::Init()
{
    GPIO::Init();
    SPI::Init();

    if(UART::Init(SCPI::OnEventCallback))
    {
        connected = true;
        running = true;
        return true;
    }

    return false;
}


void Device::Update()
{
    SCPI::Update();
}


void Device::Shutdown()
{
    running = false;

    UART::DeInit();

    connected = false;
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

#ifdef WIN32
    ComPort::Send(message, (int)std::strlen(message) + 1);
#else
    UART::SendBuffer(message, (int)std::strlen(message) + 1);
#endif
}
