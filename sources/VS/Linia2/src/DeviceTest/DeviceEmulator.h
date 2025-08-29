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

private:
    virtual ~DeviceEmulator() { }
};
