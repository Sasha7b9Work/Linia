#pragma once
#include "IPPP/Tests/SettingsTests.h"
#include "IPPP/Tests/Ranges.h"


class IDevice
{
public:

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
    virtual void SetPulseDuration(uint durationUS, GenerationStup::E) = 0;

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

    virtual void ChanBS_AmplitudeRange(Chan::E, AmplitudeRange::E) = 0;

    // Число ступеней
    virtual void ChanBS_StepCount(Chan::E, StepCount::E) = 0;

    virtual void ChanBS_AmplitudeValue(Chan::E, AmplitudeValue::E) = 0;

    // Смещение
    virtual void ChaBS_Bias(Chan::E, AmplitudeValue::E) = 0;

    // Режим измерителя
    virtual void ChanBS_MeasMode(Chan::E, ModeMeas::E) = 0;

    // Диапазон измерителя
    virtual void ChanBS_MeasRangeU(Chan::E, RangeU::E) = 0;
    virtual void ChanBS_MeasRangeI(Chan::E, RangeI::E) = 0;

    // Диапазон ограничения
    virtual void ChanBS_LimitRangeU(Chan::E, RangeU::E) = 0;
    virtual void ChanBS_LimitRangeI(Chan::E, RangeI::E) = 0;

    // Порог ограничения измерителя
    virtual void ChanBS_LimitThreshold(Chan::E, LimitThreshold::E) = 0;

    virtual void ChanBS_HighResolution(Chan::E, bool highRes) = 0;

    //------------------------------------------------------------------------------------------------------------

    virtual void SetAutoZeroOff(bool) = 0;                                          // Отключение автоматической коррекции нуля
    virtual void SetPointCount(PointCount::E) = 0;                                  // Установка количества точек измерения
    virtual void SetStartType(StartTrigger::E, StartMode::E) = 0;                   // Тип запуска: внешний/внутренний, одиночный/множественный
    virtual void SetMeasurementMode(MeasMode::E) = 0;                               // Режим измерения
    virtual void SetMaxDacCode(DacCode::E) = 0;                                     // Максимальный код DAC
    virtual void SetBitQ18(PulseState::E) = 0;                                      // Состояние бита Q18
    virtual void SetElementType(ElementType::E) = 0;                                // Тип элемента: многополюсный или нет
    virtual void SendLimitSignal(LimitResult::E) = 0;                               // Отправка сигнала ограничения
    virtual void SetConfigData(uint8_t modification, PowerNominal::E) = 0;          // Установка конфигурационных данных
    virtual void EmergencyStop() = 0;                                               // Аварийная остановка
    virtual void StartMeasurement() = 0;                                            // Запуск измерения

    virtual void StopMeasurement() = 0;                                             // Остановка измерения
    virtual void ResetToDefaults() = 0;                                             // Сброс к настройкам по умолчанию
};


extern IDevice *g_device;
