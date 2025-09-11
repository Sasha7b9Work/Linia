#include "defines.h"
#include "Device/Device.h"
#include "Device/DeviceIPPP.h"
#include "Communicator/UART/UART.h"
#include <sstream>
#include <cstdio>


IDevice *g_device = nullptr;


DeviceIPPP::DeviceIPPP() : running(false), connected(false) {}


DeviceIPPP::~DeviceIPPP() {
    Shutdown();
}

bool DeviceIPPP::Init() {
    UART::Init();
    if (UART::Open(115200, "8N1")) {
        connected = true;
        running = true;
        commThread = std::thread(&DeviceIPPP::CommunicationThread, this);
        return true;
    }
    return false;
}


void DeviceIPPP::Shutdown() {
    running = false;
    if (commThread.joinable()) {
        commThread.join();
    }
    UART::Close();
    UART::DeInit();
    connected = false;
}


bool DeviceIPPP::IsConnected() const {
    return connected && UART::IsReady();
}


void DeviceIPPP::SendCommand(const std::string& cmd) {
    std::string CMD = cmd;
    if (!CMD.empty() && CMD.back() != '\r' && CMD.back() != '\n') {
        CMD += "\r";
    }
    std::lock_guard<std::mutex> lock(queueMutex);
    commandQueue.push(CMD);
}


void DeviceIPPP::SendCommand(pchar format, ...)
{
    char message[1024];
    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    SendCommand(std::string(message));
}


void DeviceIPPP::CommunicationThread() {
    while (running) {
        std::string cmd;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (!commandQueue.empty()) {
                cmd = commandQueue.front();
                commandQueue.pop();
            }
        }
        if (!cmd.empty()) {
            UART::SendBuffer((uint8_t*)cmd.c_str(), (int)cmd.length());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}


// Реализации функций
void DeviceIPPP::SetCircuitConnection(Chan::E ch, StateJack::E state)
{
    SendCommand("%s:CONNECTION %s", Chan(ch).Name().c_str().AsChar(), StateJack::Name(state).c_str().AsChar());
}


void DeviceIPPP::SetSweepType(TypeScan::E type) {
    std::string cmd = ":TYPESCAN " + TypeScan::Name(type).ToStdString();
    SendCommand(cmd);
}


void DeviceIPPP::SetFirstQueue(FirstQueue::E fq) {
    std::string cmd = ":FIRSTQUEUE " + FirstQueue::Name(fq).ToStdString();
    SendCommand(cmd);
}


void DeviceIPPP::SetPulseDuration(uint durationUS, GenerationStup::E gs) {
    std::stringstream ss;
    ss << ":PULSE:DURATION " << durationUS << "\n:GENERATIONSTUP " << GenerationStup::Name(gs).ToStdString();
    SendCommand(ss.str());
}


void DeviceIPPP::ChanC_SourceRange(RangeU::E range) {
    std::string rangeStr = std::string(RangeU(range).Name(RowRange::_124));
    size_t spacePos = rangeStr.find(' ');
    if (spacePos != std::string::npos) {
        rangeStr.erase(spacePos, 1);
    }
    std::string cmd = ":C:RANGE:SOURCE " + rangeStr;
    SendCommand(cmd);
}


void DeviceIPPP::ChanC_MeasRange(RangeU::E range) {
    std::string rangeStr = std::string(RangeU(range).Name(RowRange::_124));
    size_t spacePos = rangeStr.find(' ');
    if (spacePos != std::string::npos) {
        rangeStr.erase(spacePos, 1);
    }
    std::string cmd = ":C:RANGE:MEAS " + rangeStr;
    SendCommand(cmd);
}


void DeviceIPPP::ChanC_MeasRange(RangeI::E range) {
    std::string rangeStr = std::string(RangeI(range).Name(RowRange::_124));
    size_t spacePos = rangeStr.find(' ');
    if (spacePos != std::string::npos) {
        rangeStr.erase(spacePos, 1);
    }
    std::string cmd = ":C:RANGE:MEAS " + rangeStr;
    SendCommand(cmd);
}


void DeviceIPPP::ChanC_LimitSourceU(int min, int max) {
    std::stringstream ss;
    ss << ":C:LIMIT " << min << " " << max;
    SendCommand(ss.str());
}


void DeviceIPPP::ChanBS_SourceMode(Chan::E ch, ModeSource::E mode)
{
    SendCommand(":%s:MODE:SOURCE %s", Chan(ch).Name().c_str().AsChar(), ModeSource::Name(mode).c_str().AsChar());
}


void DeviceIPPP::ChanBS_AmplitudeRange(Chan::E ch, RangeU::E range)
{
    SendCommand(":%s:RANGE:AMPLITUDE %s",
        Chan(ch).Name().c_str().AsChar(),
        RangeU(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_StepCount(Chan::E ch, int count)
{
    SendCommand(":%s:STEP:COUNT %d", Chan(ch).Name().c_str().AsChar(), count);
}


void DeviceIPPP::ChanBS_AmplitudeValue(Chan::E ch, int value)
{
    SendCommand("%s:AMPLITUDE %d", Chan(ch).Name().c_str().AsChar(), value);
}


void DeviceIPPP::ChaBS_Bias(Chan::E ch, int bias)
{
    SendCommand("%s:BIAS %d", Chan(ch).Name().c_str().AsChar(), bias);
}


void DeviceIPPP::ChanBS_MeasMode(Chan::E ch, ModeMeas::E mode)
{
    SendCommand(":%s:MODE:MEAS %s", Chan(ch).Name().c_str().AsChar(), ModeMeas::Name(mode).c_str().AsChar());
}


void DeviceIPPP::ChanBS_MeasRangeU(Chan::E ch, RangeU::E range)
{
//    std::string rangeStr = std::string(RangeU(range).Name(RowRange::_124));
//
//    size_t spacePos = rangeStr.find(' ');
//
//    if (spacePos != std::string::npos)
//    {
//        rangeStr.erase(spacePos, 1);
//    }

    SendCommand(":%s:RANGE:MEAS %s",
        Chan(ch).Name().c_str().AsChar(),
        RangeU(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_MeasRangeI(Chan::E ch, RangeI::E range)
{
    SendCommand(":%s:RANGE:MEAS %s",
        Chan(ch).Name().c_str().AsChar(),
        RangeI(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_LimitRangeU(Chan::E ch, RangeU::E range)
{
    SendCommand(":%s:RANGE:LIMIT %s",
        Chan(ch).Name().c_str().AsChar(),
        RangeU(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_LimitRangeI(Chan::E ch, RangeI::E range)
{
    SendCommand(":%s:RANGE:LIMIT %s",
        Chan(ch).Name().c_str().AsChar(),
        RangeI(range).Name(RowRange::ForChannel(ch), false));
}


void DeviceIPPP::ChanBS_LimitThreshold(Chan::E ch, int threshold)
{
    SendCommand(":%s:LIMIT:THRESHOLD %d", Chan(ch).Name().c_str().AsChar(), threshold);
}


void DeviceIPPP::ChanBS_HighResolution(Chan::E ch, bool highRes)
{
    std::string cmd = ":" + Chan(ch).Name().ToStdString() + ":HIGHRES " + std::string(highRes ? "1" : "0");
    SendCommand(cmd);
}


void DeviceIPPP::SetAutoZeroOff(bool off) {
    std::string cmd = ":AUTOZERO " + std::string(off ? "OFF" : "ON");
    SendCommand(cmd);
}


void DeviceIPPP::SetPointCount(int count)
{
    SendCommand(":POINTCOUNT %d", count);
}


void DeviceIPPP::SetMeasurementMode(MeasMode::E mode) {
    std::stringstream ss;
    ss << ":MEASMODE " << MeasMode::Name(mode).ToStdString();
    SendCommand(ss.str());
}


void DeviceIPPP::SetBitQ18(PulseState::E state) {
    std::stringstream ss;
    ss << ":BITQ18 " << PulseState::Name(state).ToStdString();
    SendCommand(ss.str());
}


void DeviceIPPP::SetElementType(ElementType::E isMultipole) {
    std::string cmd = ":ELEMENT " + ElementType::Name(isMultipole).ToStdString();
    SendCommand(cmd);
}


void DeviceIPPP::EmergencyStop() {
    SendCommand(":EMERGENCYSTOP");
}


void DeviceIPPP::StartMeasurement() {
    SendCommand(":STARTMEAS");
}


void DeviceIPPP::StopMeasurement() {
    SendCommand(":STOPMEAS");
}


void DeviceIPPP::ResetToDefaults() {
    SendCommand(":RESET");
}
