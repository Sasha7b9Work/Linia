// 2025/08/29 18:05:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/DeviceEmulator.h"
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


void DeviceEmulator::WriteMicroChip(MicroChip::E, int, uint)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetCircuitConnection(Chan::E, StateJack::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetSweepType(TypeScan::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetFirstQueue(Chan::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetPulseDuration(uint /*durationUS*/, bool)
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

void DeviceEmulator::ChaBS_Bias(Chan::E /*chan*/, double /*bias*/)
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


void DeviceEmulator::SetPointCount(int)
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
