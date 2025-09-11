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
void DeviceIPPP::SetCircuitConnection(Chan::E chan, StateJack::E state) {
    std::string cmd = ":S:CONNECTION " + Chan::Name(chan).ToStdString() + " " + StateJack::Name(state).ToStdString();
    SendCommand(cmd);
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


// Объединенные функции для B и S
void DeviceIPPP::ChanBS_SourceMode(Chan::E chan, ModeSource::E mode) {
    std::string cmd = ":" + Chan::Name(chan).ToStdString() + ":MODE:SOURCE " + ModeSource::Name(mode).ToStdString();
    SendCommand(cmd);
}


void DeviceIPPP::ChanBS_AmplitudeRange(Chan::E chan, AmplitudeRange::E range) {
    std::stringstream ss;
    ss << ":" << Chan::Name(chan).ToStdString() << ":RANGE:AMPLITUDE " << (int)AmplitudeRange::Value(range);
    SendCommand(ss.str());
}


void DeviceIPPP::ChanBS_StepCount(Chan::E chan, StepCount::E count) {
    std::stringstream ss;
    ss << ":" << Chan::Name(chan).ToStdString() << ":STEP:COUNT " << StepCount::Value(count);
    SendCommand(ss.str());
}


void DeviceIPPP::ChanBS_AmplitudeValue(Chan::E chan, AmplitudeValue::E value) {
    std::stringstream ss;
    ss << ":" << Chan::Name(chan).ToStdString() << ":AMPLITUDE " << AmplitudeValue::Value(value);
    SendCommand(ss.str());
}


void DeviceIPPP::ChaBS_Bias(Chan::E chan, AmplitudeValue::E bias) {
    std::stringstream ss;
    ss << ":" << Chan::Name(chan).ToStdString() << ":BIAS " << AmplitudeValue::Value(bias);
    SendCommand(ss.str());
}


void DeviceIPPP::ChanBS_MeasMode(Chan::E chan, ModeMeas::E mode) {
    std::string cmd = ":" + Chan::Name(chan).ToStdString() + ":MODE:MEAS " + ModeMeas::Name(mode).ToStdString();
    SendCommand(cmd);
}


void DeviceIPPP::ChanBS_MeasRangeU(Chan::E chan, RangeU::E range) {
    std::string rangeStr = std::string(RangeU(range).Name(RowRange::_124));
    size_t spacePos = rangeStr.find(' ');
    if (spacePos != std::string::npos) {
        rangeStr.erase(spacePos, 1);
    }
    std::string cmd = ":" + Chan::Name(chan).ToStdString() + ":RANGE:MEAS " + rangeStr;
    SendCommand(cmd);
}


void DeviceIPPP::ChanBS_MeasRangeI(Chan::E chan, RangeI::E range) {
    std::string rangeStr = std::string(RangeI(range).Name(RowRange::_124));
    size_t spacePos = rangeStr.find(' ');
    if (spacePos != std::string::npos) {
        rangeStr.erase(spacePos, 1);
    }
    std::string cmd = ":" + Chan::Name(chan).ToStdString() + ":RANGE:MEAS " + rangeStr;
    SendCommand(cmd);
}


void DeviceIPPP::ChanBS_LimitRangeU(Chan::E chan, RangeU::E range) {
    std::string rangeStr = std::string(RangeU(range).Name(RowRange::_124));
    size_t spacePos = rangeStr.find(' ');
    if (spacePos != std::string::npos) {
        rangeStr.erase(spacePos, 1);
    }
    std::string cmd = ":" + Chan::Name(chan).ToStdString() + ":RANGE:LIMIT " + rangeStr;
    SendCommand(cmd);
}


void DeviceIPPP::ChanBS_LimitRangeI(Chan::E chan, RangeI::E range) {
    std::string rangeStr = std::string(RangeI(range).Name(RowRange::_124));
    size_t spacePos = rangeStr.find(' ');
    if (spacePos != std::string::npos) {
        rangeStr.erase(spacePos, 1);
    }
    std::string cmd = ":" + Chan::Name(chan).ToStdString() + ":RANGE:LIMIT " + rangeStr;
    SendCommand(cmd);
}


void DeviceIPPP::ChanBS_LimitThreshold(Chan::E chan, LimitThreshold::E threshold) {
    std::stringstream ss;
    ss << ":" << Chan::Name(chan).ToStdString() << ":LIMIT:THRESHOLD " << LimitThreshold::Value(threshold);
    SendCommand(ss.str());
}


void DeviceIPPP::ChanBS_HighResolution(Chan::E chan, bool highRes) {
    std::string cmd = ":" + Chan::Name(chan).ToStdString() + ":HIGHRES " + std::string(highRes ? "1" : "0");
    SendCommand(cmd);
}


void DeviceIPPP::SetAutoZeroOff(bool off) {
    std::string cmd = ":AUTOZERO " + std::string(off ? "OFF" : "ON");
    SendCommand(cmd);
}


void DeviceIPPP::SetPointCount(PointCount::E count) {
    std::stringstream ss;
    ss << ":POINTCOUNT " << PointCount::Name(count).ToStdString();
    SendCommand(ss.str());
}


void DeviceIPPP::SetStartType(StartTrigger::E external, StartMode::E multiple) {
    std::string cmd = ":START " + StartTrigger::Name(external).ToStdString() + " " + StartMode::Name(multiple).ToStdString();
    SendCommand(cmd);
}


void DeviceIPPP::SetMeasurementMode(MeasMode::E mode) {
    std::stringstream ss;
    ss << ":MEASMODE " << MeasMode::Name(mode).ToStdString();
    SendCommand(ss.str());
}


void DeviceIPPP::SetMaxDacCode(DacCode::E code) {
    std::stringstream ss;
    ss << ":MAXDAC " << DacCode::Name(code).ToStdString();
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


void DeviceIPPP::SendLimitSignal(LimitResult::E limitCode) {
    std::stringstream ss;
    ss << ":LIMITSIGNAL " << LimitResult::Name(limitCode).ToStdString();
    SendCommand(ss.str());
}


void DeviceIPPP::SetConfigData(uint8_t modification, PowerNominal::E nominal) {
    std::stringstream ss;
    ss << ":CONFIG " << (int)modification << " " << PowerNominal::Name(nominal).ToStdString();
    SendCommand(ss.str());
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
