// 2025/08/29 18:05:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/Device/IDevice.h"


class DeviceEmulator : public IDevice
{
public:

    virtual bool Init() override;

    virtual void Shutdown() override;

    virtual bool IsConnected() const;

    virtual void WriteMicroChip(MicroChip::E, int reg, uint value) override;

private:

    virtual ~DeviceEmulator() { }
};
