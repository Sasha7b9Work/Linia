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
    virtual void SchemeInclusion(StateJack::E chC, StateJack::E chB, StateJack::E chS) = 0;     // 1

    //------------------------------------------------------------------------------------------------------------

    // Диапазон источника U в канале C
    virtual void ChannelC_RangeSourceU(RangeU::E) = 0;                  // 5

    // Диапазон измерителя U в канале C
    virtual void ChannelC_RangeMeasU(RangeU::E) = 0;                    // 6

    // Диапазон измерителя I в канале C
    virtual void ChannelC_RangeMeasI(RangeI::E) = 0;                    // 7

    // Ограничение источника U от 0 до 100%
    virtual void ChannelC_LimitSourceU(int min, int max) = 0;           // 8

    //------------------------------------------------------------------------------------------------------------

    // Режим источника в каналах B, S
    virtual void ChannelBS_ModeSource(Channel::E, ModeSource::E) = 0;   // 9    19

    // Диапазон амплитуды ступени в канале B
    virtual void ChannelB_RangeStep(RangeU::E) = 0;                     // 10
    virtual void ChannelB_RangeStep(RangeI::E) = 0;

    // Число ступеней в канале B
    virtual void ChannelB_NumberSteps(int) = 0;                         // 11

    // Значение амплитуды ступени в канале B
    virtual void ChannelB_AmplitudeStep(int16) = 0;                     // 12

    // Включение/выключение автоустановки нуля (или калибровки)
    virtual void ChannelB_AutoSetZero(bool) = 0;                        // 13

    // Смещение в канале B
    virtual void ChannelB_Offset(int16) = 0;                            // 14

    // Режим измерителя в канале B
    virtual void ChannelB_ModeMeas(ModeMeas::E) = 0;                    // 15

    // Диапазон измерителя
    virtual void ChannelB_RangeMeas(RangeU::E) = 0;                     // 16
    virtual void ChannelB_RangeMeas(RangeI::E) = 0;

    // Диапазон ограничение в канале B
    virtual void ChannelB_RangeLimit(RangeU::E) = 0;                    // 17
    virtual void ChannelB_RangeLimit(RangeI::E) = 0;

    // Порог ограничения измерителя в канале B
    virtual void ChannelB_ThresholdLimit(int16) = 0;                    // 18

    //------------------------------------------------------------------------------------------------------------

    // Диапазон амплитуды ступени в канале S
    virtual void ChannelS_RangeStep(RangeU::E) = 0;                     // 20
    virtual void ChannelS_RangeStep(RangeI::E) = 0;

    // Число ступеней в канале S
    virtual void ChannelS_NumberSteps(int) = 0;                         // 21

    // Значение амплитуды ступени в канале S
    virtual void ChannelS_AmplitudeStep(int16) = 0;                     // 22

    // Смещение в канале S
    virtual void ChannelS_Offset(int16) = 0;                            // 24

    // Режим измерителя в канале S
    virtual void ChannelS_ModeMeas(ModeMeas::E) = 0;                    // 25

    // Диапазон измерителя в канале S
    virtual void ChannelS_RangeMeas(RangeU::E) = 0;                     // 26
    virtual void ChannelS_RangeMeas(RangeI::E) = 0;

    virtual ~IDeviceTest() { }
};


extern IDeviceTest *devicet;
