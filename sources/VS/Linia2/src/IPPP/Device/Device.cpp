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
    UART::Init();

    if (UART::Open(115200, "8N1"))
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


void Device::SetFirstQueue(const Chan &ch)
{
    SendCommand(":FIRSTQUEUE %s", ch.Name());
}


void Device::SetPulseDuration(uint durationUS, bool genertaion_stump)
{
    SendCommand(":PULSE:DURATION %u", durationUS);
    SendCommand(":GENERATIONSTUP %d", genertaion_stump ? 1 : 0);
}


void Device::StartMeasurement()
{
    SendCommand(":MEAS:START");
}


void Device::StopMeasurement()
{
    SendCommand(":MEAS:STOP");
}


void Device::WriteMicroChip(MicroChip::E chip, int reg, uint value)
{
    SendCommand(":CHIP:%s:%d:WRITE %08X", MicroChip::Name(chip), reg, value);
}
