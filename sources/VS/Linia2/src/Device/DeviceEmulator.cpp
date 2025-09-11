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


void DeviceEmulator::SetCircuitConnection(const Chan &, StateJack::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetTypeScan(TypeScan::E)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::SetFirstQueue(const Chan &)
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


void DeviceEmulator::ChanBS_SourceMode(const Chan &/*chan*/, ModeSource::E /*mode*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_RangeStep(const Chan &/*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_RangeStep(const Chan &, RangeI::E)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_StepCount(const Chan &/*chan*/, int /*count*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_AmplitudeValue(const Chan &/*chan*/, int /*value*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChaBS_Offset(const Chan &/*chan*/, double)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_MeasMode(const Chan &/*chan*/, ModeMeas::E /*mode*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_MeasRange(const Chan &/*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_MeasRange(const Chan &/*chan*/, RangeI::E /*range*/)
{
    LOG_ERROR("Not implemented");
}

void DeviceEmulator::ChanBS_LimitRange(const Chan &/*chan*/, RangeU::E /*range*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::ChanBS_LimitRange(const Chan &/*chan*/, RangeI::E /*range*/)
{
    LOG_ERROR("Not implemented");
}


void DeviceEmulator::ChanBS_LimitThreshold(const Chan &/*chan*/, int /*threshold*/)
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
