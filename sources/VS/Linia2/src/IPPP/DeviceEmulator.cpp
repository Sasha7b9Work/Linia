// 2025/08/29 18:05:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "IPPP/DeviceEmulator.h"
#include "Communicator/GPIO/GPIO.h"
#include "Communicator/SPI/SPI.h"
#include "Communicator/UART/UART.h"


bool DeviceEmulator::Init()
{
    GPIO::Init();
    SPI::Init();
    UART::Init();

    LOG_ERROR("Not implemented");

    return false;
}


void DeviceEmulator::DeviceEmulator::Shutdown()
{
    LOG_ERROR("Not implemented");
}


bool DeviceEmulator::IsConnected() const
{
    LOG_ERROR("Not implemented");

    return false;
}


void DeviceEmulator::SetCircuitConnection(Chan::E, StateJack::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetSweepType(TypeScan::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetFirstQueue(FirstQueue::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetPulseDuration(uint /*durationUS*/, GenerationStup::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::ChanC_SourceRange(RangeU::E)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanC_MeasRange(RangeU::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::ChanC_MeasRange(RangeI::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::ChanC_LimitSourceU(int /*min*/, int /*max*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::ChanBS_SourceMode(Chan::E /*chan*/, ModeSource::E /*mode*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_AmplitudeRange(Chan::E /*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_StepCount(Chan::E /*chan*/, int /*count*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_AmplitudeValue(Chan::E /*chan*/, int /*value*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChaBS_Bias(Chan::E /*chan*/, int /*bias*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_MeasMode(Chan::E /*chan*/, ModeMeas::E /*mode*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_MeasRangeU(Chan::E /*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_MeasRangeI(Chan::E /*chan*/, RangeI::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_LimitRangeU(Chan::E /*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_LimitRangeI(Chan::E /*chan*/, RangeI::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_LimitThreshold(Chan::E /*chan*/, int /*threshold*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_HighResolution(Chan::E /*chan*/, bool /*highRes*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetAutoZeroOff(bool)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetPointCount(PointCount::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetStartType(StartTrigger::E /*external*/, StartMode::E /*multiple*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetMeasurementMode(MeasMode::E /*mode*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetMaxDacCode(DacCode::E /*code*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetBitQ18(PulseState::E /*state*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetElementType(ElementType::E /*isMultipole*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::EmergencyStop()
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::StartMeasurement()
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::StopMeasurement()
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::ResetToDefaults()
{
    LOG_ERROR("Not implemented");
}
