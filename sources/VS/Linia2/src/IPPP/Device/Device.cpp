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


void Device::ChanBS_SourceMode(const Chan &ch, ModeSource::E mode)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:SOURCE:MODE %s", ch.Name(), ModeSource::Name(mode));
    }
}


void Device::ChanBS_StepRange(const Chan &ch, RangeU::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:STEP:RANGE %s",
            ch.Name(),
            RangeU(range).Name(RowRange(ch.value), false));
    }
}


void Device::ChanBS_StepRange(const Chan &ch, RangeI::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:STEP:RANGE %s",
            ch.Name(),
            RangeI(range).Name(RowRange(ch.value), false));
    }
}


void Device::ChanBS_StepCount(const Chan &ch, int count)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:STEP:COUNT %d", ch.Name(), count);
    }
}


void Device::ChanBS_StepValue(const Chan &ch, double value)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:STEP:VALUE %10e", ch.Name(), value);
    }
}


void Device::ChaBS_Offset(const Chan &ch, double offset)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:OFFSET %10e", ch.Name(), offset);
    }
}


void Device::ChanBS_MeasMode(const Chan &ch, ModeMeas::E mode)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:MEAS:MODE %s", ch.Name(), ModeMeas::Name(mode));
    }
}


void Device::ChanBS_MeasRange(const Chan &ch, RangeU::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:MEAS:RANGE %s",
            ch.Name(),
            RangeU(range).Name(RowRange(ch.value), false));
    }
}


void Device::ChanBS_MeasRange(const Chan &ch, RangeI::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:MEAS:RANGE %s",
            ch.Name(),
            RangeI(range).Name(RowRange(ch.value), false));
    }
}


void Device::ChanBS_LimitRange(const Chan &ch, RangeU::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:LIMIT:RANGE %s",
            ch.Name(),
            RangeU(range).Name(RowRange(ch.value), false));
    }
}


void Device::ChanBS_LimitRange(const Chan &ch, RangeI::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:LIMIT:RANGE %s",
            ch.Name(),
            RangeI(range).Name(RowRange(ch.value), false));
    }
}


void Device::ChanBS_LimitThreshold(const Chan &ch, int threshold)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:LIMIT:THRESHOLD %d", ch.Name(), threshold);
    }
}


bool Device::IsChanBS(const Chan &ch) const
{
    if (ch.IsBS())
    {
        return true;
    }

    LOG_ERROR("Channel must be B or S, not %s", ch.Name());

    return false;
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
