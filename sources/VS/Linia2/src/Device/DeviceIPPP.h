// 2025/09/11 08:48:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Device/Device.h"


class DeviceIPPP : public IDevice
{
public:

    DeviceIPPP();

    virtual ~DeviceIPPP();

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
    virtual void SetChannel_SourceMode(Chan::E, ModeSource::E) override;

    virtual void SetChannel_AmplitudeRange(Chan::E, AmplitudeRange::E) override;

    // Число ступеней
    virtual void SetChannel_StepCount(Chan::E, StepCount::E) override;

    virtual void SetChannel_AmplitudeValue(Chan::E, AmplitudeValue::E) override;

    // Смещение
    virtual void SetChannel_Bias(Chan::E, AmplitudeValue::E) override;

    // Режим измерителя
    virtual void SetChannel_MeasMode(Chan::E, ModeMeas::E) override;

    // Диапазон измерителя
    virtual void SetChannel_MeasRangeU(Chan::E, RangeU::E) override;
    virtual void SetChannel_MeasRangeI(Chan::E, RangeI::E) override;

    // Диапазон ограничения
    virtual void SetChannel_LimitRangeU(Chan::E, RangeU::E) override;
    virtual void SetChannel_LimitRangeI(Chan::E, RangeI::E) override;

    // Порог ограничения измерителя
    virtual void SetChannel_LimitThreshold(Chan::E, LimitThreshold::E) override;

    virtual void SetChannel_HighResolution(Chan::E, bool highRes) override;

    //------------------------------------------------------------------------------------------------------------

    virtual void SetAutoZeroOff(bool) override;
    virtual void SetPointCount(PointCount::E) override;
    virtual void SetStartType(StartTrigger::E, StartMode::E) override;
    virtual void SetMeasurementMode(MeasMode::E) override;
    virtual void SetMaxDacCode(DacCode::E) override;
    virtual void SetBitQ18(PulseState::E) override;
    virtual void SetElementType(ElementType::E) override;
    virtual void SendLimitSignal(LimitResult::E) override;
    virtual void SetConfigData(uint8_t modification, PowerNominal::E) override;
    virtual void EmergencyStop() override;
    virtual void StartMeasurement() override;

    virtual void StopMeasurement() override;
    virtual void ResetToDefaults() override;


private:

    void SendCommand(const std::string &cmd);
    void CommunicationThread();

    std::queue<std::string> commandQueue;
    std::mutex queueMutex;
    std::thread commThread;
    std::atomic<bool> running;
    std::atomic<bool> connected;
};
