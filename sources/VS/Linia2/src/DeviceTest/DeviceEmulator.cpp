// 2025/08/29 18:05:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "DeviceTest/DeviceEmulator.h"


void DeviceEmulator::Init()
{

}


void DeviceEmulator::Start()
{

}


void DeviceEmulator::Stop()
{

}


void DeviceEmulator::SetCallbackReadData(void (* /*func*/)(int16))
{

}


void DeviceEmulator::SchemeInclusion(StateJack::E /*chC*/, StateJack::E /*chB*/, StateJack::E /*chS*/)
{

}


void DeviceEmulator::WriteTypeScan(TypeScan::E, FirstQueue::E)
{

}


void DeviceEmulator::WriteDurationImpulseUS(uint /*durationUS*/, GenerationStup::E)
{

}


//------------------------------------------------------------------------------------------------------------


void DeviceEmulator::ChannelC_RangeSourceU(RangeU::E)
{

}


void DeviceEmulator::ChannelC_RangeMeas(RangeU::E)
{

}


void DeviceEmulator::ChannelC_RangeMeas(RangeI::E)
{

}


void DeviceEmulator::ChannelC_LimitSourceU(int /*min*/, int /*max*/)
{

}


void DeviceEmulator::ChannelBS_ModeSource(Chan::E, ModeSource::E)
{

}


void DeviceEmulator::ChannelBS_RangeStep(Chan::E, RangeU::E)
{

}


void DeviceEmulator::ChannelBS_RangeStep(Chan::E, RangeI::E)
{

}


void DeviceEmulator::ChannelBS_NumberSteps(Chan::E, int)
{

}


void DeviceEmulator::ChannelBS_AmplitudeStep(Chan::E, int16)
{

}


void DeviceEmulator::AutoSetZero(bool)
{

}


void DeviceEmulator::ChannelBS_Offset(Chan::E, int16)
{

}


void DeviceEmulator::ChannelBS_ModeMeas(Chan::E, ModeMeas::E)
{

}


void DeviceEmulator::ChannelBS_RangeMeas(Chan::E, RangeU::E)
{

}


void DeviceEmulator::ChannelBS_RangeMeas(Chan::E, RangeI::E)
{

}


void DeviceEmulator::ChannelBS_RangeLimit(Chan::E, RangeU::E)
{

}


void DeviceEmulator::ChannelBS_RangeLimit(Chan::E, RangeI::E)
{

}


void DeviceEmulator::ChannelBS_ThresholdLimit(Chan::E, int16)
{

}


void DeviceEmulator::NumberPoints(int)
{

}
