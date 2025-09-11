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
    virtual void SetFirstQueue(Chan::E) override;

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

    // Режим источника в канале
    virtual void ChanBS_SourceMode(Chan::E, ModeSource::E) override;

    virtual void ChanBS_AmplitudeRange(Chan::E, RangeU::E) override;

    // Число ступеней
    virtual void ChanBS_StepCount(Chan::E, int) override;

    virtual void ChanBS_AmplitudeValue(Chan::E, int) override;

    // Смещение
    virtual void ChaBS_Bias(Chan::E, int) override;

    // Режим измерителя
    virtual void ChanBS_MeasMode(Chan::E, ModeMeas::E) override;

    // Диапазон измерителя
    virtual void ChanBS_MeasRangeU(Chan::E, RangeU::E) override;
    virtual void ChanBS_MeasRangeI(Chan::E, RangeI::E) override;

    // Диапазон ограничения
    virtual void ChanBS_LimitRangeU(Chan::E, RangeU::E) override;
    virtual void ChanBS_LimitRangeI(Chan::E, RangeI::E) override;

    // Порог ограничения измерителя
    virtual void ChanBS_LimitThreshold(Chan::E, int) override;

    //------------------------------------------------------------------------------------------------------------

    virtual void SetPointCount(int) override;

    virtual void StartMeasurement() override;
    virtual void StopMeasurement() override;

private:

    void SendCommand(pchar format, ...);
    void CommunicationThread();

    std::queue<std::string> commandQueue;
    std::mutex queueMutex;
    std::thread commThread;
    std::atomic<bool> running;
    std::atomic<bool> connected;
};
