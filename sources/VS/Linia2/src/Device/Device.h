#pragma once
#include "IPPP/Tests/SettingsTests.h"
#include "IPPP/Tests/Ranges.h"


class IDevice
{
public:

    static IDevice *impl;

    virtual ~IDevice() = default;

    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
    virtual bool IsConnected() const = 0;

    // Схема включения канала
    virtual void SetCircuitConnection(Chan::E, StateJack::E) = 0;

    // Вид развёртки
    virtual void SetSweepType(TypeScan::E) = 0;

    // Первая очередь
    virtual void SetFirstQueue(FirstQueue::E) = 0;

    // Длительность импульса
    // generation_stump - семейство кривых
    virtual void SetPulseDuration(uint durationUS, bool generation_stump) = 0;

    //------------------------------------------------------------------------------------------------------------

    // Диапазон источника
    virtual void ChanC_SourceRange(RangeU::E) = 0;

    // Диапазон измерителя
    virtual void ChanC_MeasRange(RangeU::E) = 0;
    virtual void ChanC_MeasRange(RangeI::E) = 0;

    // Ограничение источника U от 0 до 100%
    virtual void ChanC_LimitSourceU(int min, int max) = 0;

    //------------------------------------------------------------------------------------------------------------

    // Функции для каналов B и S

    // Режим источника в канале
    virtual void ChanBS_SourceMode(Chan::E, ModeSource::E) = 0;

    virtual void ChanBS_AmplitudeRange(Chan::E, RangeU::E) = 0;

    // Число ступеней
    virtual void ChanBS_StepCount(Chan::E, int) = 0;

    virtual void ChanBS_AmplitudeValue(Chan::E, int) = 0;

    // Смещение
    virtual void ChaBS_Bias(Chan::E, int) = 0;

    // Режим измерителя
    virtual void ChanBS_MeasMode(Chan::E, ModeMeas::E) = 0;

    // Диапазон измерителя
    virtual void ChanBS_MeasRangeU(Chan::E, RangeU::E) = 0;
    virtual void ChanBS_MeasRangeI(Chan::E, RangeI::E) = 0;

    // Диапазон ограничения
    virtual void ChanBS_LimitRangeU(Chan::E, RangeU::E) = 0;
    virtual void ChanBS_LimitRangeI(Chan::E, RangeI::E) = 0;

    // Порог ограничения измерителя
    virtual void ChanBS_LimitThreshold(Chan::E, int) = 0;

    //------------------------------------------------------------------------------------------------------------

    virtual void SetPointCount(int) = 0;                            // Установка количества точек измерения

    virtual void StartMeasurement() = 0;                            // Запуск измерения
    virtual void StopMeasurement() = 0;                             // Остановка измерения
};
