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
    virtual void SetChannelC_SourceRange(RangeU::E);

    // Диапазон измерителя
    virtual void SetChannelC_MeasRange(RangeU::E);
    virtual void SetChannelC_MeasRange(RangeI::E);

    // Ограничение источника U от 0 до 100%
    virtual void SetChannelC_LimitSourceU(int min, int max);

    //------------------------------------------------------------------------------------------------------------

    // Функции для каналов B и S

    virtual void SetChannel_SourceMode(Chan::E chan, ModeSource::E mode) override;
    virtual void SetChannel_AmplitudeRange(Chan::E chan, AmplitudeRange::E range) override;
    virtual void SetChannel_StepCount(Chan::E chan, StepCount::E count) override;
    virtual void SetChannel_AmplitudeValue(Chan::E chan, AmplitudeValue::E value) override;
    virtual void SetChannel_Bias(Chan::E chan, AmplitudeValue::E bias) override;
    virtual void SetChannel_MeasMode(Chan::E chan, ModeMeas::E mode) override;
    virtual void SetChannel_MeasRangeU(Chan::E chan, RangeU::E range) override;
    virtual void SetChannel_MeasRangeI(Chan::E chan, RangeI::E range) override;
    virtual void SetChannel_LimitRangeU(Chan::E chan, RangeU::E range) override;
    virtual void SetChannel_LimitRangeI(Chan::E chan, RangeI::E range) override;
    virtual void SetChannel_LimitThreshold(Chan::E chan, LimitThreshold::E threshold) override;
    virtual void SetChannel_HighResolution(Chan::E chan, bool highRes) override;



    //------------------------------------------------------------------------------------------------------------

    virtual void SetAutoZeroOff(bool) override;
    virtual void SetPointCount(PointCount::E) override;
    virtual void SetStartType(StartTrigger::E, StartMode::E) override;
    virtual void SetMeasurementMode(MeasMode::E mode) override;
    virtual void SetMaxDacCode(DacCode::E code) override;
    virtual void SetBitQ18(PulseState::E state) override;
    virtual void SetElementType(ElementType::E isMultipole) override;
    virtual void SendLimitSignal(LimitResult::E limitCode) override;
    virtual void SetConfigData(uint8_t modification, PowerNominal::E nominal) override;
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
