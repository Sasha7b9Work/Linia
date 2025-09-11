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


void DeviceEmulator::SetChannelC_SourceRange(RangeU::E)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannelC_MeasRange(RangeU::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetChannelC_MeasRange(RangeI::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetChannelC_LimitSourceU(int /*min*/, int /*max*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetChannel_SourceMode(Chan::E /*chan*/, ModeSource::E /*mode*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_AmplitudeRange(Chan::E /*chan*/, AmplitudeRange::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_StepCount(Chan::E /*chan*/, StepCount::E /*count*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_AmplitudeValue(Chan::E /*chan*/, AmplitudeValue::E /*value*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_Bias(Chan::E /*chan*/, AmplitudeValue::E /*bias*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_MeasMode(Chan::E /*chan*/, ModeMeas::E /*mode*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_MeasRangeU(Chan::E /*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_MeasRangeI(Chan::E /*chan*/, RangeI::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_LimitRangeU(Chan::E /*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_LimitRangeI(Chan::E /*chan*/, RangeI::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_LimitThreshold(Chan::E /*chan*/, LimitThreshold::E /*threshold*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::SetChannel_HighResolution(Chan::E /*chan*/, bool /*highRes*/)
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


void DeviceEmulator::SendLimitSignal(LimitResult::E /*limitCode*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetConfigData(uint8_t /*modification*/, PowerNominal::E /*nominal*/)
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
