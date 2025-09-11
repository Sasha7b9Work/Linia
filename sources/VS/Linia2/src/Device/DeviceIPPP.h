// 2025/09/11 08:48:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Device/IDevice.h"


class DeviceIPPP : public IDevice
{
public:

    DeviceIPPP() { }

    virtual ~DeviceIPPP();

    virtual bool Init() override;

    virtual void Shutdown() override;

    virtual bool IsConnected() const override;

    virtual void SetPointCount(int) override;

    virtual void StartMeasurement() override;
    virtual void StopMeasurement() override;

    virtual void WriteMicroChip(MicroChip::E, int reg, uint value) override;

    virtual void SetCircuitConnection(const Chan &, StateJack::E) override;

    virtual void SetTypeScan(TypeScan::E) override;

    virtual void SetFirstQueue(const Chan &) override;

    virtual void SetPulseDuration(uint durationUS, bool generation_stump) override;

    //------------------------------------------------------------------------------------------------------------

    virtual void ChanC_SourceRange(RangeU::E) override;

    virtual void ChanC_MeasRange(RangeU::E) override;
    virtual void ChanC_MeasRange(RangeI::E) override;

    virtual void ChanC_LimitSourceU(int min, int max) override;

    //------------------------------------------------------------------------------------------------------------

    virtual void ChanBS_SourceMode(const Chan &, ModeSource::E) override;

    virtual void ChanBS_StepRange(const Chan &, RangeU::E) override;
    virtual void ChanBS_StepRange(const Chan &, RangeI::E) override;

    virtual void ChanBS_StepCount(const Chan &, int) override;

    virtual void ChanBS_StepValue(const Chan &, double) override;

    virtual void ChaBS_Offset(const Chan &, double) override;

    virtual void ChanBS_MeasMode(const Chan &, ModeMeas::E) override;

    virtual void ChanBS_MeasRange(const Chan &, RangeU::E) override;
    virtual void ChanBS_MeasRange(const Chan &, RangeI::E) override;

    virtual void ChanBS_LimitRange(const Chan &, RangeU::E) override;
    virtual void ChanBS_LimitRange(const Chan &, RangeI::E) override;

    virtual void ChanBS_LimitThreshold(const Chan &, double) override;

    //------------------------------------------------------------------------------------------------------------

private:

    void SendCommand(pchar format, ...);

    bool IsChanBS(const Chan &) const;

    std::atomic<bool> running = false;
    std::atomic<bool> connected = false;
};
