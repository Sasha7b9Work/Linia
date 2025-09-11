// 2025/08/29 18:05:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Device/Device.h"


class DeviceEmulator : public IDevice
{
public:

    virtual bool Init();
    virtual void Shutdown();
    virtual bool IsConnected() const;

    // Схема включения канала
    virtual void SetCircuitConnection(Chan::E, StateJack::E);

    // Вид развёртки
    virtual void SetSweepType(TypeScan::E);

    // Первая очередь
    virtual void SetFirstQueue(FirstQueue::E);

    // Длительность импульса
    virtual void SetPulseDuration(uint durationUS, GenerationStup::E);

    //------------------------------------------------------------------------------------------------------------

    // Диапазон источника
    virtual void ChanC_SourceRange(RangeU::E);

    // Диапазон измерителя
    virtual void ChanC_MeasRange(RangeU::E);
    virtual void ChanC_MeasRange(RangeI::E);

    // Ограничение источника U от 0 до 100%
    virtual void ChanC_LimitSourceU(int min, int max);

    //------------------------------------------------------------------------------------------------------------

    // Функции для каналов B и S

    virtual void ChanBS_SourceMode(Chan::E, ModeSource::E) override;
    virtual void ChanBS_AmplitudeRange(Chan::E, RangeU::E) override;
    virtual void ChanBS_StepCount(Chan::E, int) override;
    virtual void ChanBS_AmplitudeValue(Chan::E, int) override;
    virtual void ChaBS_Bias(Chan::E, int) override;
    virtual void ChanBS_MeasMode(Chan::E, ModeMeas::E) override;
    virtual void ChanBS_MeasRangeU(Chan::E, RangeU::E) override;
    virtual void ChanBS_MeasRangeI(Chan::E, RangeI::E) override;
    virtual void ChanBS_LimitRangeU(Chan::E, RangeU::E) override;
    virtual void ChanBS_LimitRangeI(Chan::E, RangeI::E) override;
    virtual void ChanBS_LimitThreshold(Chan::E, int) override;
    virtual void ChanBS_HighResolution(Chan::E, bool highRes) override;



    //------------------------------------------------------------------------------------------------------------

    virtual void SetAutoZeroOff(bool) override;
    virtual void SetPointCount(PointCount::E) override;
    virtual void SetStartType(StartTrigger::E, StartMode::E) override;
    virtual void SetMeasurementMode(MeasMode::E) override;
    virtual void SetMaxDacCode(DacCode::E) override;
    virtual void SetBitQ18(PulseState::E) override;
    virtual void SetElementType(ElementType::E) override;
    virtual void EmergencyStop();
    virtual void StartMeasurement();

    virtual void StopMeasurement();
    virtual void ResetToDefaults();

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
