// 2025/09/11 08:48:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/Device/IDevice.h"


class Device : public IDevice
{
public:

    Device() { }

    virtual ~Device();

    virtual bool Init() override;

    virtual void Shutdown() override;

    virtual bool IsConnected() const override;

    virtual void StartMeasurement() override;
    virtual void StopMeasurement() override;

    virtual void WriteMicroChip(MicroChip::E, int reg, uint value) override;

    virtual void SetFirstQueue(const Chan &) override;

    virtual void SetPulseDuration(uint durationUS, bool generation_stump) override;

private:

    void SendCommand(pchar format, ...);

    std::atomic<bool> running = false;
    std::atomic<bool> connected = false;
};
