// 2025/08/29 18:05:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Device/IDevice.h"


class DeviceEmulator : public IDevice
{
public:

    virtual bool Init() override;
    virtual void Shutdown() override;
    virtual bool IsConnected() const;

    virtual void WriteMicroChip(MicroChip::E, int reg, uint value) override;

    // Схема включения канала
    virtual void SetCircuitConnection(const Chan &, StateJack::E) override;

    // Вид развёртки
    virtual void SetTypeScan(TypeScan::E) override;

    // Первая очередь
    virtual void SetFirstQueue(const Chan &) override;

    // Длительность импульса
    virtual void SetPulseDuration(uint durationUS, bool generation_stump) override;

    //------------------------------------------------------------------------------------------------------------

    // Диапазон источника
    virtual void ChanC_SourceRange(RangeU::E) override;

    // Диапазон измерителя
    virtual void ChanC_MeasRange(RangeU::E) override;
    virtual void ChanC_MeasRange(RangeI::E) override;

    // Ограничение источника U от 0 до 100%
    virtual void ChanC_LimitSourceU(int min, int max) override;

    //------------------------------------------------------------------------------------------------------------

    // Функции для каналов B и S

    virtual void ChanBS_SourceMode(const Chan &, ModeSource::E) override;
    virtual void ChanBS_RangeStep(const Chan &, RangeU::E) override;
    virtual void ChanBS_RangeStep(const Chan &, RangeI::E) override;
    virtual void ChanBS_StepCount(const Chan &, int) override;
    virtual void ChanBS_AmplitudeValue(const Chan &, int) override;
    virtual void ChaBS_Offset(const Chan &, double) override;
    virtual void ChanBS_MeasMode(const Chan &, ModeMeas::E) override;
    virtual void ChanBS_MeasRange(const Chan &, RangeU::E) override;
    virtual void ChanBS_MeasRange(const Chan &, RangeI::E) override;
    virtual void ChanBS_LimitRange(const Chan &, RangeU::E) override;
    virtual void ChanBS_LimitRange(const Chan &, RangeI::E) override;
    virtual void ChanBS_LimitThreshold(const Chan &, int) override;

    //------------------------------------------------------------------------------------------------------------

    virtual void SetPointCount(int) override;

    virtual void StartMeasurement() override;
    virtual void StopMeasurement() override;

private:

    virtual ~DeviceEmulator() { }
};
