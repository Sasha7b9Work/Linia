// 2025/08/29 18:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tests/SettingsTests.h"
#include "Tests/Ranges.h"


class IDeviceTest
{
public:
    virtual void Init() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;

    // Функция, которая будет передавать принятые данные
    virtual void SetCallbackReadData(void (*func)(int16)) = 0;

    // Схема включения всех каналов
    virtual void SchemeInclusion(StateJack::E chC, StateJack::E chB, StateJack::E chS) = 0;

    //------------------------------------------------------------------------------------------------------------

    // Диапазон источника U в канале C
    virtual void ChannelC_RangeSourceU(RangeU::E) = 0;

    // Диапазон измерителя U в канале C
    virtual void ChannelC_RangeMeasU(RangeU::E) = 0;

    // Диапазон измерителя I в канале C
    virtual void ChannelC_RangeMeasI(RangeI::E) = 0;

    // Ограничение источника U от 0 до 100%
    virtual void ChannelC_LimitSourceU(int min, int max) = 0;

    // Режим источника в канале B
    virtual void ChannelB_ModeSource(ModeSource::E) = 0;

    // Диапазон амплитуды ступени в канале B
    virtual void ChannelB_AmplitudeStep(RangeU::E) = 0;
    virtual void ChannelB_AmplitudeStep(RangeI::E) = 0;

    // Число ступеней в канале B
    virtual void ChannelB_NumberSteps(int) = 0;

    virtual ~IDeviceTest() { }
};


extern IDeviceTest *devicet;
