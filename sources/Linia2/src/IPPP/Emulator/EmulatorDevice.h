// 2025/08/29 18:05:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "IPPP/IDevice.h"


class EmulatorDevice : public IDevice
{
public:

    EmulatorDevice() = default;

    virtual ~EmulatorDevice();

    virtual bool Init() override;

    virtual void ApplicationTask() override;

    virtual void Shutdown() override;

    virtual bool IsConnected() const override;

    virtual void SendCommand(pchar format, ...) const override;

private:

    static void CallbackOnReceive(uint8 *, int);
};
