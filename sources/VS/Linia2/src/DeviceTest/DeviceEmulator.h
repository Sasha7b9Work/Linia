// 2025/08/29 18:05:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "DeviceTest/DeviceTest.h"


class DeviceEmulator : public IDeviceTest
{
public:

    virtual void Init() override;

    virtual void Start() override;

    virtual void Stop() override;

    virtual void SetCallbackReadData(void (*func)(int16));

    virtual void SchemeInclusion(StateJack::E chC, StateJack::E chB, StateJack::E chS);

    //------------------------------------------------------------------------------------------------------------

    virtual void ChannelC_RangeSourceU(RangeU::E);

    virtual void ChannelC_RangeMeasU(RangeU::E);

    virtual void ChannelC_RangeMeasI(RangeI::E);

    virtual void ChannelC_LimitSourceU(int min, int max);

    //------------------------------------------------------------------------------------------------------------

    virtual void ChannelBS_ModeSource(Channel::E, ModeSource::E);

    virtual void ChannelB_RangeStep(RangeU::E);
    virtual void ChannelB_RangeStep(RangeI::E);

    virtual void ChannelB_NumberSteps(int);

    virtual void ChannelB_AmplitudeStep(int16);

    virtual void ChannelB_AutoSetZero(bool);

    virtual void ChannelB_Offset(int16);

    virtual void ChannelB_ModeMeas(ModeMeas::E);

    virtual void ChannelB_RangeMeas(RangeU::E);
    virtual void ChannelB_RangeMeas(RangeI::E);

    virtual void ChannelB_RangeLimit(RangeU::E);
    virtual void ChannelB_RangeLimit(RangeI::E);

    virtual void ChannelB_ThresholdLimit(int16);

    //------------------------------------------------------------------------------------------------------------

    virtual void ChannelS_RangeStep(RangeU::E);
    virtual void ChannelS_RangeStep(RangeI::E);

    virtual void ChannelS_NumberSteps(int);

    virtual void ChannelS_AmplitudeStep(int16);

    virtual void ChannelS_Offset(int16);

    virtual void ChannelS_ModeMeas(ModeMeas::E);

    virtual void ChannelS_RangeMeas(RangeU::E);
    virtual void ChannelS_RangeMeas(RangeI::E);

private:
    virtual ~DeviceEmulator() { }
};
