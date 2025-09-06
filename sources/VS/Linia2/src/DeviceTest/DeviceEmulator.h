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

    // Режим источника в канале B
    virtual void SetChannelB_SourceMode(ModeSource::E);

    virtual void SetChannelB_AmplitudeRange(uint8_t range);

    // Число ступеней
    virtual void SetChannelB_StepCount(int);

    virtual void SetChannelB_AmplitudeValue(int16_t);

    // Смещение
    virtual void SetChannelB_Bias(pchar);

    // Режим измерителя
    virtual void SetChannelB_MeasMode(ModeMeas::E);

    // Диапазон измерителя
    virtual void SetChannelB_MeasRange(RangeU::E);
    virtual void SetChannelB_MeasRange(RangeI::E);

    // Диапазон ограничения
    virtual void SetChannelB_LimitRange(RangeU::E);
    virtual void SetChannelB_LimitRange(RangeI::E);

    // Порог ограничения измерителя
    virtual void SetChannelB_LimitThreshold(int16_t);

    virtual void SetChannelB_HighResolution(bool);

    //------------------------------------------------------------------------------------------------------------

    virtual void SetChannelS_SourceMode(ModeSource::E);
    virtual void SetChannelS_AmplitudeRange(uint8_t range);
    virtual void SetChannelS_StepCount(uint8_t steps);
    virtual void SetChannelS_AmplitudeValue(uint16_t value);
    virtual void SetChannelS_Bias(uint16_t value);
    virtual void SetChannelS_MeasMode(ModeSource::E);
    virtual void SetChannelS_MeasRange(uint8_t range);
    virtual void SetChannelS_LimitRange(uint8_t range);
    virtual void SetChannelS_LimitThreshold(uint16_t value);
    virtual void SetChannelS_SourceType(ModeSource::E);

    //------------------------------------------------------------------------------------------------------------

    virtual void SetAutoZeroOff(bool);
    virtual void SetPointCount(int);
    virtual void SetStartType(bool external, bool multiple);
    virtual void SetMeasurementMode(uint8_t mode);
    virtual void SetMaxDacCode(uint8_t code);
    virtual void SetBitQ18(uint8_t state);
    virtual void SetElementType(bool isMultipole);
    virtual void SendLimitSignal(uint8_t limitCode);
    virtual void SetConfigData(uint8_t modification, uint8_t nominal);
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
