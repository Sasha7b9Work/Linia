// 2025/08/29 18:05:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/IDevice.h"


class EmulatorDevice : public IDevice
{
public:

    virtual bool Init() override;

    virtual void Shutdown() override;

    virtual bool IsConnected() const;

private:

    virtual ~EmulatorDevice() { }

    static void CallbackOnReceive(uint8 *, int);
};
