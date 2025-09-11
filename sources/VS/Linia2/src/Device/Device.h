#pragma once

// Стандартные заголовки
#include <atomic>
#include <sstream>
#include <queue>
#include <mutex>
#include <thread>
#include <string>
#include <vector>

// Заголовки проекта
#include "../defines.h"
#include "../IPPP/Tests/SettingsTests.h"
#include "../IPPP/Tests/Ranges.h"
    
class IDevice {
public:
    virtual ~IDevice() = default;

    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
    virtual bool IsConnected() const = 0;

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

    // Функции для каналов B и S

    // Режим источника в канале
    virtual void SetChannel_SourceMode(Chan::E chan, ModeSource::E mode) = 0; // :(B/S):MODE:SOURCE U

    virtual void SetChannel_AmplitudeRange(Chan::E chan, AmplitudeRange::E range) = 0; // :(B/S):RANGE:AMPLITUDE <range>

    // Число ступеней
    virtual void SetChannel_StepCount(Chan::E chan, StepCount::E count) = 0; // :(B/S):STEP:COUNT <count>

    virtual void SetChannel_AmplitudeValue(Chan::E chan, AmplitudeValue::E value) = 0; // :(B/S):AMPLITUDE <value>

    // Смещение
    virtual void SetChannel_Bias(Chan::E chan, AmplitudeValue::E bias) = 0; // :(B/S):BIAS <value>

    // Режим измерителя
    virtual void SetChannel_MeasMode(Chan::E chan, ModeMeas::E mode) = 0; // :(B/S):MODE:MEAS U

    // Диапазон измерителя
    virtual void SetChannel_MeasRangeU(Chan::E chan, RangeU::E range) = 0; // :(B/S):RANGE:MEAS <range>
    virtual void SetChannel_MeasRangeI(Chan::E chan, RangeI::E range) = 0; // :(B/S):RANGE:MEAS <range>

    // Диапазон ограничения
    virtual void SetChannel_LimitRangeU(Chan::E chan, RangeU::E range) = 0; // :(B/S):RANGE:LIMIT <range>
    virtual void SetChannel_LimitRangeI(Chan::E chan, RangeI::E range) = 0; // :(B/S):RANGE:LIMIT <range>

    // Порог ограничения измерителя
    virtual void SetChannel_LimitThreshold(Chan::E chan, LimitThreshold::E threshold) = 0; // :(B/S):LIMIT:THRESHOLD <threshold>

    virtual void SetChannel_HighResolution(Chan::E chan, bool highRes) = 0; // :(B/S):HIGHRES <0|1>
    //------------------------------------------------------------------------------------------------------------

    virtual void SetAutoZeroOff(bool) = 0;                                    // Отключение автоматической коррекции нуля
    virtual void SetPointCount(PointCount::E) = 0;                        // Установка количества точек измерения
    virtual void SetStartType(StartTrigger::E, StartMode::E) = 0;         // Тип запуска: внешний/внутренний, одиночный/множественный
    virtual void SetMeasurementMode(MeasMode::E mode) = 0;                // Режим измерения
    virtual void SetMaxDacCode(DacCode::E code) = 0;                      // Максимальный код DAC
    virtual void SetBitQ18(PulseState::E state) = 0;                      // Состояние бита Q18
    virtual void SetElementType(ElementType::E isMultipole) = 0;          // Тип элемента: многополюсный или нет
    virtual void SendLimitSignal(LimitResult::E limitCode) = 0;                   // Отправка сигнала ограничения
    virtual void SetConfigData(uint8_t modification, PowerNominal::E nominal) = 0; // Установка конфигурационных данных
    virtual void EmergencyStop() = 0;                                      // Аварийная остановка
    virtual void StartMeasurement() = 0;                                   // Запуск измерения

    virtual void StopMeasurement() = 0;                                    // Остановка измерения
    virtual void ResetToDefaults() = 0;                                    // Сброс к настройкам по умолчанию
};


class Device : public IDevice {
public:
    Device();
    virtual ~Device();

    virtual bool Init() override;
    virtual void Shutdown() override;
    virtual bool IsConnected() const override;

    // Схема включения канала
    virtual void SetCircuitConnection(Chan::E, StateJack::E) override;

    // Вид развёртки
    virtual void SetSweepType(TypeScan::E) override;

    // Первая очередь
    virtual void SetFirstQueue(FirstQueue::E) override;

    // Длительность импульса
    virtual void SetPulseDuration(uint durationUS, GenerationStup::E) override;

    //------------------------------------------------------------------------------------------------------------

    // Диапазон источника
    virtual void SetChannelC_SourceRange(RangeU::E) override;

    // Диапазон измерителя
    virtual void SetChannelC_MeasRange(RangeU::E) override;
    virtual void SetChannelC_MeasRange(RangeI::E) override;

    // Ограничение источника U от 0 до 100%
    virtual void SetChannelC_LimitSourceU(int min, int max) override;

    //------------------------------------------------------------------------------------------------------------

    // Функции для каналов B и S

    // Режим источника в канале
    virtual void SetChannel_SourceMode(Chan::E chan, ModeSource::E mode) override;
    
    virtual void SetChannel_AmplitudeRange(Chan::E chan, AmplitudeRange::E range) override;

    // Число ступеней
    virtual void SetChannel_StepCount(Chan::E chan, StepCount::E count) override;
    
    virtual void SetChannel_AmplitudeValue(Chan::E chan, AmplitudeValue::E value) override;

    // Смещение
    virtual void SetChannel_Bias(Chan::E chan, AmplitudeValue::E bias) override;

    // Режим измерителя
    virtual void SetChannel_MeasMode(Chan::E chan, ModeMeas::E mode) override;

    // Диапазон измерителя
    virtual void SetChannel_MeasRangeU(Chan::E chan, RangeU::E range) override;
    virtual void SetChannel_MeasRangeI(Chan::E chan, RangeI::E range) override;

    // Диапазон ограничения
    virtual void SetChannel_LimitRangeU(Chan::E chan, RangeU::E range) override;
    virtual void SetChannel_LimitRangeI(Chan::E chan, RangeI::E range) override;

    // Порог ограничения измерителя
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
    virtual void EmergencyStop() override;
    virtual void StartMeasurement() override;

    virtual void StopMeasurement() override;
    virtual void ResetToDefaults() override;
    

private:
    void SendCommand(const std::string& cmd);
    void CommunicationThread();

    std::queue<std::string> commandQueue;
    std::mutex queueMutex;
    std::thread commThread;
    std::atomic<bool> running;
    std::atomic<bool> connected;
};


extern IDevice *g_device;
