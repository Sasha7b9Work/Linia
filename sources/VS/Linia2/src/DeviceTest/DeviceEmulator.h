// 2025/08/29 18:05:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "DeviceTest/DeviceTest.h"


class DeviceEmulator : public IDeviceTest
{
public:

    virtual void Init() override;

    virtual void Start() override;

    virtual void Stop() override;

    virtual void SetCallbackReadData(void (*)(int16));

    virtual void SchemeInclusion(StateJack::E chC, StateJack::E chB, StateJack::E chS);

    //------------------------------------------------------------------------------------------------------------

    virtual void ChannelC_RangeSourceU(RangeU::E);

    virtual void ChannelC_RangeMeas(RangeU::E);
    virtual void ChannelC_RangeMeas(RangeI::E);

    virtual void ChannelC_LimitSourceU(int min, int max);

    //------------------------------------------------------------------------------------------------------------

    virtual void ChannelBS_ModeSource(Channel::E, ModeSource::E);

    virtual void ChannelBS_RangeStep(Channel::E, RangeU::E);
    virtual void ChannelBS_RangeStep(Channel::E, RangeI::E);

    virtual void ChannelBS_NumberSteps(Channel::E, int);

    virtual void ChannelBS_AmplitudeStep(Channel::E, int16);

    virtual void ChannelBS_Offset(Channel::E, int16);

    virtual void ChannelBS_ModeMeas(Channel::E, ModeMeas::E);

    virtual void ChannelBS_RangeMeas(Channel::E, RangeU::E);
    virtual void ChannelBS_RangeMeas(Channel::E, RangeI::E);

    virtual void ChannelBS_RangeLimit(Channel::E, RangeU::E);
    virtual void ChannelBS_RangeLimit(Channel::E, RangeI::E);

    virtual void ChannelBS_ThresholdLimit(Channel::E, int16);

    //------------------------------------------------------------------------------------------------------------

    virtual void NumberPoints(int);

    virtual void AutoSetZero(bool);

private:
    virtual ~DeviceEmulator() { }
};
