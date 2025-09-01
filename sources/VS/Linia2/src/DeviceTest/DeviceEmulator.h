// 2025/08/29 18:05:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "DeviceTest/DeviceTest.h"


class DeviceEmulator : public IDeviceTest
{
public:

    virtual void Init();

    virtual void Start();

    virtual void Stop();

    virtual void SetCallbackReadData(void (*)(int16));

    virtual void SchemeInclusion(StateJack::E chC, StateJack::E chB, StateJack::E chS);

    virtual void WriteTypeScan(TypeScan::E, FirstQueue::E);

    virtual void WriteDurationImpulseUS(uint durationUS, GenerationStup::E);

    //------------------------------------------------------------------------------------------------------------

    virtual void ChannelC_RangeSourceU(RangeU::E);

    virtual void ChannelC_RangeMeas(RangeU::E);
    virtual void ChannelC_RangeMeas(RangeI::E);

    virtual void ChannelC_LimitSourceU(int min, int max);

    //------------------------------------------------------------------------------------------------------------

    virtual void ChannelBS_ModeSource(Chan::E, ModeSource::E);

    virtual void ChannelBS_RangeStep(Chan::E, RangeU::E);
    virtual void ChannelBS_RangeStep(Chan::E, RangeI::E);

    virtual void ChannelBS_NumberSteps(Chan::E, int);

    virtual void ChannelBS_AmplitudeStep(Chan::E, int16);

    virtual void ChannelBS_Offset(Chan::E, int16);

    virtual void ChannelBS_ModeMeas(Chan::E, ModeMeas::E);

    virtual void ChannelBS_RangeMeas(Chan::E, RangeU::E);
    virtual void ChannelBS_RangeMeas(Chan::E, RangeI::E);

    virtual void ChannelBS_RangeLimit(Chan::E, RangeU::E);
    virtual void ChannelBS_RangeLimit(Chan::E, RangeI::E);

    virtual void ChannelBS_ThresholdLimit(Chan::E, int16);

    //------------------------------------------------------------------------------------------------------------

    virtual void NumberPoints(int);

    virtual void AutoSetZero(bool);

private:

    virtual ~DeviceEmulator() { }

    void (*callback_read_data)(int16) = nullptr;
    StateJack::E jackC;
    StateJack::E jackB;
    StateJack::E jackS;

    // Диапазон источника напряжения
    RangeU::E range_source[Chan::Count];

    // Диапазон измерителя напряжения
    RangeU::E range_measU[Chan::Count];

    // Диапазон измерителя тока
    RangeI::E range_measI[Chan::Count];

    // Ограничение источника U в канале C в процентах
    wxRange limit_sourceC;
};
