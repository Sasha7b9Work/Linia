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

    // Записать значение в регистр.
    // Если размер регистра меньше 32, записываются младшие биты
    virtual void WriteMicroChip(MicroChip::E, int reg, uint value) = 0;

    // Схема включения канала
    virtual void SetCircuitConnection(const Chan &, StateJack::E) = 0;

    // Вид развёртки
    virtual void SetTypeScan(TypeScan::E) = 0;

    // Первая очередь
    virtual void SetFirstQueue(const Chan &) = 0;

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
    virtual void ChanBS_SourceMode(const Chan &, ModeSource::E) = 0;

    // Диапазон амплитуды ступени в канале
    virtual void ChanBS_RangeStep(const Chan &, RangeU::E) = 0;
    virtual void ChanBS_RangeStep(const Chan &, RangeI::E) = 0;

    // Число ступеней
    virtual void ChanBS_StepCount(const Chan &, int) = 0;

    // Значение амплитуды ступени
    virtual void ChanBS_ValueStep(const Chan &, double) = 0;

    // Смещение
    virtual void ChaBS_Offset(const Chan &, double) = 0;

    // Режим измерителя
    virtual void ChanBS_MeasMode(const Chan &, ModeMeas::E) = 0;

    // Диапазон измерителя
    virtual void ChanBS_MeasRange(const Chan &, RangeU::E) = 0;
    virtual void ChanBS_MeasRange(const Chan &, RangeI::E) = 0;

    // Диапазон ограничения
    virtual void ChanBS_LimitRange(const Chan &, RangeU::E) = 0;
    virtual void ChanBS_LimitRange(const Chan &, RangeI::E) = 0;

    // Порог ограничения измерителя
    virtual void ChanBS_LimitThreshold(const Chan &, double) = 0;

    //------------------------------------------------------------------------------------------------------------

    virtual void SetPointCount(int) = 0;                            // Установка количества точек измерения

    virtual void StartMeasurement() = 0;                            // Запуск измерения
    virtual void StopMeasurement() = 0;                             // Остановка измерения
};
