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
        commThread = std::thread(&DeviceIPPP::CommunicationThread, this);
        return true;
    }

    return false;
}


void DeviceIPPP::Shutdown()
{
    running = false;

    if (commThread.joinable())
    {
        commThread.join();
    }

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

    std::string CMD = message;

    if (!CMD.empty() && CMD.back() != '\r' && CMD.back() != '\n')
    {
        CMD += "\r";
    }

    std::lock_guard<std::mutex> lock(queueMutex);

    commandQueue.push(CMD);
}


void DeviceIPPP::CommunicationThread()
{
    while (running)
    {
        std::string cmd;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (!commandQueue.empty())
            {
                cmd = commandQueue.front();
                commandQueue.pop();
            }
        }

        if (!cmd.empty())
        {
            UART::SendBuffer((uint8*)cmd.c_str(), (int)cmd.length());
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


void DeviceIPPP::SetCircuitConnection(Chan::E ch, StateJack::E state)
{
    SendCommand("%s:CONNECTION %s",
        Chan(ch).Name(),
        StateJack::NameHardware(state));
}


void DeviceIPPP::SetSweepType(TypeScan::E type)
{
    SendCommand(":TYPESCAN %s", TypeScan::Name(type));
}


void DeviceIPPP::SetFirstQueue(Chan::E ch)
{
    SendCommand(":FIRSTQUEUE %s", + Chan(ch).Name());
}


void DeviceIPPP::SetPulseDuration(uint durationUS, bool genertaion_stump)
{
    SendCommand(":PULSE:DURATION %u", durationUS);
    SendCommand(":GENERATIONSTUP %d", genertaion_stump ? 1 : 0);
}


void DeviceIPPP::ChanC_SourceRange(RangeU::E range)
{
    SendCommand(":C:RANGE:SOURCE %s", RangeU(range).Name(RowRange::ForChannel(Chan::_C), false));
}


void DeviceIPPP::ChanC_MeasRange(RangeU::E range)
{
    SendCommand(":C:RANGE:MEAS %s", RangeU(range).Name(RowRange::ForChannel(Chan::_C), false));
}


void DeviceIPPP::ChanC_MeasRange(RangeI::E range)
{
    SendCommand(":C:RANGE:MEAS %s", RangeI(range).Name(RowRange::ForChannel(Chan::_C), false));
}


void DeviceIPPP::ChanC_LimitSourceU(int min, int max)
{
    SendCommand(":C:LIMIT %d %d", min, max);
}


void DeviceIPPP::ChanBS_SourceMode(Chan::E ch, ModeSource::E mode)
{
    SendCommand(":%s:MODE:SOURCE %s", Chan(ch).Name(), ModeSource::Name(mode));
}


void DeviceIPPP::ChanBS_AmplitudeRange(Chan::E ch, RangeU::E range)
{
    SendCommand(":%s:RANGE:AMPLITUDE %s",
        Chan(ch).Name(),
        RangeU(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_StepCount(Chan::E ch, int count)
{
    SendCommand(":%s:STEP:COUNT %d", Chan(ch).Name(), count);
}


void DeviceIPPP::ChanBS_AmplitudeValue(Chan::E ch, int value)
{
    SendCommand("%s:AMPLITUDE %d", Chan(ch).Name(), value);
}


void DeviceIPPP::ChaBS_Bias(Chan::E ch, int bias)
{
    SendCommand("%s:BIAS %d", Chan(ch).Name(), bias);
}


void DeviceIPPP::ChanBS_MeasMode(Chan::E ch, ModeMeas::E mode)
{
    SendCommand(":%s:MODE:MEAS %s", Chan(ch).Name(), ModeMeas::Name(mode));
}


void DeviceIPPP::ChanBS_MeasRangeU(Chan::E ch, RangeU::E range)
{
    SendCommand(":%s:RANGE:MEAS %s",
        Chan(ch).Name(),
        RangeU(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_MeasRangeI(Chan::E ch, RangeI::E range)
{
    SendCommand(":%s:RANGE:MEAS %s",
        Chan(ch).Name(),
        RangeI(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_LimitRangeU(Chan::E ch, RangeU::E range)
{
    SendCommand(":%s:RANGE:LIMIT %s",
        Chan(ch).Name(),
        RangeU(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_LimitRangeI(Chan::E ch, RangeI::E range)
{
    SendCommand(":%s:RANGE:LIMIT %s",
        Chan(ch).Name(),
        RangeI(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_LimitThreshold(Chan::E ch, int threshold)
{
    SendCommand(":%s:LIMIT:THRESHOLD %d", Chan(ch).Name(), threshold);
}


void DeviceIPPP::SetPointCount(int count)
{
    SendCommand(":POINTCOUNT %d", count);
}


void DeviceIPPP::StartMeasurement()
{
    SendCommand(":STARTMEAS");
}


void DeviceIPPP::StopMeasurement()
{
    SendCommand(":STOPMEAS");
}
