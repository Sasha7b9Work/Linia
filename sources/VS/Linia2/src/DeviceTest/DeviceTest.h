// 2025/08/29 18:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tests/SettingsTests.h"
#include "Tests/Ranges.h"


class IDeviceTest
{
public:
    virtual void Init() = 0;
    virtual void Start() = 0;                                           // 41
    virtual void Stop() = 0;                                            // 38

    // Функция, которая будет передавать принятые данные
    virtual void SetCallbackReadData(void (*func)(int16)) = 0;

    // Схема включения всех каналов
    virtual void SchemeInclusion(StateJack::E chC, StateJack::E chB, StateJack::E chS) = 0;     // 1

    //------------------------------------------------------------------------------------------------------------

    // Диапазон источника U в канале C
    virtual void ChannelC_RangeSourceU(RangeU::E) = 0;                  // 4

    // Диапазон измерителя в канале C
    virtual void ChannelC_RangeMeas(RangeU::E) = 0;                     // 5
    virtual void ChannelC_RangeMeas(RangeI::E) = 0;                     // 6

    // Ограничение источника U от 0 до 100%
    virtual void ChannelC_LimitSourceU(int min, int max) = 0;           // 7, 8

    //------------------------------------------------------------------------------------------------------------

    // Режим источника в каналах B, S
    virtual void ChannelBS_ModeSource(Channel::E, ModeSource::E) = 0;   // 9    19

    // Диапазон амплитуды ступени в каналах B, S
    virtual void ChannelBS_RangeStep(Channel::E, RangeU::E) = 0;        // 10   20
    virtual void ChannelBS_RangeStep(Channel::E, RangeI::E) = 0;

    // Число ступеней в каналах B, S
    virtual void ChannelBS_NumberSteps(Channel::E, int) = 0;            // 11   21

    // Значение амплитуды ступени в каналах B, S
    virtual void ChannelBS_AmplitudeStep(Channel::E, int16) = 0;        // 12   22

    // Смещение в каналах B, S
    virtual void ChannelBS_Offset(Channel::E, int16) = 0;               // 14   24

    // Режим измерителя в каналах B, S
    virtual void ChannelBS_ModeMeas(Channel::E, ModeMeas::E) = 0;       // 15   25

    // Диапазон измерителя
    virtual void ChannelBS_RangeMeas(Channel::E, RangeU::E) = 0;        // 16   26
    virtual void ChannelBS_RangeMeas(Channel::E, RangeI::E) = 0;

    // Диапазон ограничение в каналах B, S
    virtual void ChannelBS_RangeLimit(Channel::E, RangeU::E) = 0;       // 17   27
    virtual void ChannelBS_RangeLimit(Channel::E, RangeI::E) = 0;

    // Порог ограничения измерителя в каналах B, S
    virtual void ChannelBS_ThresholdLimit(Channel::E, int16) = 0;       // 18   28

    //------------------------------------------------------------------------------------------------------------

    // Число точек в режиме DC или импульсном. Может быть 20 или 50
    virtual void NumberPoints(int) = 0;                                 // 30

    // Включение/выключение автоустановки нуля (или калибровки)
    virtual void AutoSetZero(bool) = 0;                                 // 13   31

    virtual ~IDeviceTest() { }
};


extern IDeviceTest *devicet;
