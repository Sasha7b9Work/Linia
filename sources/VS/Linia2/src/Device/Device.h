#pragma once
#include "IPPP/Tests/SettingsTests.h"
#include "IPPP/Tests/Ranges.h"


// \todo Дописать комментарии на функции неочевидного назначения
// \todo В каналах B и S одинаковые функции. Их нужно объединить и первым аргументом передавать канал


class IDevice {
public:
    virtual ~IDevice() = default;

    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
    virtual bool IsConnected() const = 0;

    //    virtual void SetDataCallback(DataCallback callback) = 0;

    // Схема включения канала
    virtual void SetCircuitConnection(Chan::E, StateJack::E) = 0;               // :S:CONNECTION <C,B,S,E,GEN,1K,BREAK>

    // Вид развёртки
    virtual void SetSweepType(TypeScan::E) = 0;                                 // :TYPESCAN

    // Первая очередь
    virtual void SetFirstQueue(FirstQueue::E) = 0;                              // :FIRSTQUEUE <B,C>

    // Длительность импульса
    virtual void SetPulseDuration(uint durationUS, GenerationStup::E) = 0;      // :PULSE:DURATION 200
                                                                                // :GENERATIONSTUP 1

    //------------------------------------------------------------------------------------------------------------

    // Диапазон источника
    virtual void SetChannelC_SourceRange(RangeU::E) = 0;                        // :C:RANGE:SOURCE 50mV

    // Диапазон измерителя
    virtual void SetChannelC_MeasRange(RangeU::E) = 0;                          // :C:RANGE:MEAS 10V
    virtual void SetChannelC_MeasRange(RangeI::E) = 0;                          // :C:RANGE:MEAS 50uA

    // Ограничение источника U от 0 до 100%
    virtual void SetChannelC_LimitSourceU(int min, int max) = 0;                // :C:LIMIT 10 20

    //------------------------------------------------------------------------------------------------------------

    // Режим источника в канале B
    virtual void SetChannelB_SourceMode(ModeSource::E) = 0;                     // :S:MODE:SOURCE U

    virtual void SetChannelB_AmplitudeRange(uint8_t range) = 0;

    // Число ступеней
    virtual void SetChannelB_StepCount(int) = 0;                                // :B:STEP:COUNT 8

    virtual void SetChannelB_AmplitudeValue(int16_t) = 0;

    // Смещение
    virtual void SetChannelB_Bias(pchar) = 0;                                   // :S:BIAS 10.2mV

    // Режим измерителя
    virtual void SetChannelB_MeasMode(ModeMeas::E) = 0;                         // :B:MODE:MEAS I

    // Диапазон измерителя
    virtual void SetChannelB_MeasRange(RangeU::E) = 0;                          // :B:RANGE:MEAS 10V
    virtual void SetChannelB_MeasRange(RangeI::E) = 0;                          // :B:RANGE:MEAS 1pA

    // Диапазон ограничения
    virtual void SetChannelB_LimitRange(RangeU::E) = 0;                         // :S:RANGE:LIMIT 10V
    virtual void SetChannelB_LimitRange(RangeI::E) = 0;                         // :B:RANGE:LIMIT 40A

    // Порог ограничения измерителя
    virtual void SetChannelB_LimitThreshold(int16_t) = 0;

    virtual void SetChannelB_HighResolution(bool) = 0;

    //------------------------------------------------------------------------------------------------------------

    virtual void SetChannelS_SourceMode(ModeSource::E) = 0;
    virtual void SetChannelS_AmplitudeRange(uint8_t range) = 0;
    virtual void SetChannelS_StepCount(uint8_t steps) = 0;
    virtual void SetChannelS_AmplitudeValue(uint16_t value) = 0;
    virtual void SetChannelS_Bias(uint16_t value) = 0;
    virtual void SetChannelS_MeasMode(ModeSource::E) = 0;
    virtual void SetChannelS_MeasRange(uint8_t range) = 0;
    virtual void SetChannelS_LimitRange(uint8_t range) = 0;
    virtual void SetChannelS_LimitThreshold(uint16_t value) = 0;
    virtual void SetChannelS_SourceType(ModeSource::E) = 0;

    //------------------------------------------------------------------------------------------------------------

    virtual void SetAutoZeroOff(bool) = 0;
    virtual void SetPointCount(int) = 0;
    virtual void SetStartType(bool external, bool multiple) = 0;
    virtual void SetMeasurementMode(uint8_t mode) = 0;
    virtual void SetMaxDacCode(uint8_t code) = 0;
    virtual void SetBitQ18(uint8_t state) = 0;
    virtual void SetElementType(bool isMultipole) = 0;
    virtual void SendLimitSignal(uint8_t limitCode) = 0;
    virtual void SetConfigData(uint8_t modification, uint8_t nominal) = 0;
    virtual void EmergencyStop() = 0;
    virtual void StartMeasurement() = 0;

    virtual void StopMeasurement() = 0;
    virtual void ResetToDefaults() = 0;
};


extern IDevice *g_device;
