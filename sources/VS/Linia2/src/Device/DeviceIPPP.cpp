#include "defines.h"
#include "Device/IDevice.h"
#include "Device/DeviceIPPP.h"
#include "Communicator/UART/UART.h"
#include <sstream>
#include <cstdio>


IDevice *IDevice::impl = nullptr;


DeviceIPPP::DeviceIPPP() :
    running(false), connected(false)
{
}


DeviceIPPP::~DeviceIPPP()
{
    Shutdown();
}

bool DeviceIPPP::Init()
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


void DeviceIPPP::Shutdown()
{
    running = false;

    UART::Close();
    UART::DeInit();

    connected = false;
}


bool DeviceIPPP::IsConnected() const
{
    return connected && UART::IsReady();
}


void DeviceIPPP::SendCommand(pchar format, ...)
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    std::strcat(message, "\r");

    UART::SendBuffer(message, (int)std::strlen(message));
}


void DeviceIPPP::SetCircuitConnection(Chan &ch, StateJack::E state)
{
    SendCommand("%s:CONNECTION %s",
        ch.Name(),
        StateJack::NameHardware(state));
}


void DeviceIPPP::SetSweepType(TypeScan::E type)
{
    SendCommand(":TYPESCAN %s", TypeScan::Name(type));
}


void DeviceIPPP::SetFirstQueue(Chan &ch)
{
    SendCommand(":FIRSTQUEUE %s", ch.Name());
}


void DeviceIPPP::SetPulseDuration(uint durationUS, bool genertaion_stump)
{
    SendCommand(":PULSE:DURATION %u", durationUS);
    SendCommand(":GENERATIONSTUP %d", genertaion_stump ? 1 : 0);
}


void DeviceIPPP::ChanC_SourceRange(RangeU::E range)
{
    SendCommand(":C:RANGE:SOURCE %s", RangeU(range).Name(RowRange(Chan::_C), false));
}


void DeviceIPPP::ChanC_MeasRange(RangeU::E range)
{
    SendCommand(":C:RANGE:MEAS %s", RangeU(range).Name(RowRange(Chan::_C), false));
}


void DeviceIPPP::ChanC_MeasRange(RangeI::E range)
{
    SendCommand(":C:RANGE:MEAS %s", RangeI(range).Name(RowRange(Chan::_C), false));
}


void DeviceIPPP::ChanC_LimitSourceU(int min, int max)
{
    SendCommand(":C:LIMIT %d %d", min, max);
}


void DeviceIPPP::ChanBS_SourceMode(Chan::E ch, ModeSource::E mode)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:MODE:SOURCE %s", Chan(ch).Name(), ModeSource::Name(mode));
    }
}


void DeviceIPPP::ChanBS_AmplitudeRange(Chan::E ch, RangeU::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:RANGE:AMPLITUDE %s",
            Chan(ch).Name(),
            RangeU(range).Name(RowRange(ch), false));
    }
}


void DeviceIPPP::ChanBS_StepCount(Chan::E ch, int count)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:STEP:COUNT %d", Chan(ch).Name(), count);
    }
}


void DeviceIPPP::ChanBS_AmplitudeValue(Chan::E ch, int value)
{
    if (IsChanBS(ch))
    {
        SendCommand("%s:AMPLITUDE %d", Chan(ch).Name(), value);
    }
}


void DeviceIPPP::ChaBS_Bias(Chan::E ch, double bias)
{
    if (IsChanBS(ch))
    {
        SendCommand("%s:BIAS %10e", Chan(ch).Name(), bias);
    }
}


void DeviceIPPP::ChanBS_MeasMode(Chan::E ch, ModeMeas::E mode)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:MODE:MEAS %s", Chan(ch).Name(), ModeMeas::Name(mode));
    }
}


void DeviceIPPP::ChanBS_MeasRange(Chan::E ch, RangeU::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:RANGE:MEAS %s",
            Chan(ch).Name(),
            RangeU(range).Name(RowRange(ch), false));
    }
}


void DeviceIPPP::ChanBS_MeasRange(Chan::E ch, RangeI::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:RANGE:MEAS %s",
            Chan(ch).Name(),
            RangeI(range).Name(RowRange(ch), false));
    }
}


void DeviceIPPP::ChanBS_LimitRange(Chan::E ch, RangeU::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:RANGE:LIMIT %s",
            Chan(ch).Name(),
            RangeU(range).Name(RowRange(ch), false));
    }
}


void DeviceIPPP::ChanBS_LimitRange(Chan::E ch, RangeI::E range)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:RANGE:LIMIT %s",
            Chan(ch).Name(),
            RangeI(range).Name(RowRange(ch), false));
    }
}


void DeviceIPPP::ChanBS_LimitThreshold(Chan::E ch, int threshold)
{
    if (IsChanBS(ch))
    {
        SendCommand(":%s:LIMIT:THRESHOLD %d", Chan(ch).Name(), threshold);
    }
}


bool DeviceIPPP::IsChanBS(Chan::E ch) const
{
    if (ch == Chan::_B || ch == Chan::_S)
    {
        return true;
    }

    LOG_ERROR("Channel must be B or S");

    return false;
}


void DeviceIPPP::SetPointCount(int count)
{
    SendCommand(":POINTCOUNT %d", count);
}


void DeviceIPPP::StartMeasurement()
{
    SendCommand(":MEAS:START");
}


void DeviceIPPP::StopMeasurement()
{
    SendCommand(":MEAS:STOP");
}


void DeviceIPPP::WriteMicroChip(MicroChip::E chip, int reg, uint value)
{
    SendCommand(":CHIP:%s:%d:WRITE %08X", MicroChip::Name(chip), reg, value);
}
