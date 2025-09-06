// 2025/08/29 18:05:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "DeviceTest/DeviceEmulator.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"
#include "Communicator/UART/UART.h"


bool DeviceEmulator::Init()
{
    GPIO::Init();
    SPI::Init();
    UART::Init();

    LOG_WRITE("Not implemented");

    return false;
}


void DeviceEmulator::DeviceEmulator::Shutdown()
{
    LOG_WRITE("Not implemented");
}


bool DeviceEmulator::IsConnected() const
{
    LOG_WRITE("Not implemented");

    return false;
}


void DeviceEmulator::SetCircuitConnection(Chan::E, StateJack::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetSweepType(TypeScan::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetFirstQueue(FirstQueue::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetPulseDuration(uint /*durationUS*/, GenerationStup::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelC_SourceRange(RangeU::E)
{
    LOG_WRITE("Not implemented");
}

void DeviceEmulator::SetChannelC_MeasRange(RangeU::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelC_MeasRange(RangeI::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelC_LimitSourceU(int /*min*/, int /*max*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_SourceMode(ModeSource::E)
{
    LOG_WRITE("Not implemented");
}

void DeviceEmulator::SetChannelB_AmplitudeRange(uint8_t /*range*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_StepCount(int)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_AmplitudeValue(int16_t)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_Bias(pchar)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_MeasMode(ModeMeas::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_MeasRange(RangeU::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_MeasRange(RangeI::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_LimitRange(RangeU::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_LimitRange(RangeI::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_LimitThreshold(int16_t)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelB_HighResolution(bool)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_SourceMode(ModeSource::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_AmplitudeRange(uint8_t /*range*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_StepCount(uint8_t /*steps*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_AmplitudeValue(uint16_t /*value*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_Bias(uint16_t /*value*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_MeasMode(ModeSource::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_MeasRange(uint8_t /*range*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_LimitRange(uint8_t /*range*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_LimitThreshold(uint16_t /*value*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetChannelS_SourceType(ModeSource::E)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetAutoZeroOff(bool)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetPointCount(int)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetStartType(bool /*external*/, bool /*multiple*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetMeasurementMode(uint8_t /*mode*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetMaxDacCode(uint8_t /*code*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetBitQ18(uint8_t /*state*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetElementType(bool /*isMultipole*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SendLimitSignal(uint8_t /*limitCode*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::SetConfigData(uint8_t /*modification*/, uint8_t /*nominal*/)
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::EmergencyStop()
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::StartMeasurement()
{
    LOG_WRITE("Not implemented");
}

void DeviceEmulator::StopMeasurement()
{
    LOG_WRITE("Not implemented");
}


void DeviceEmulator::ResetToDefaults()
{
    LOG_WRITE("Not implemented");
}
