// 2025/08/29 18:05:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "DeviceTest/DeviceTest.h"


class DeviceEmulator : public IDeviceTest
{
public:

    virtual void Init() override;

    virtual void Start() override;

    virtual void Stop() override;

    virtual void SetCallbackReadData(void (*func)(int16));

    virtual void WriteSchemeInclusion(StateJack::E chC, StateJack::E chB, StateJack::E chS);

private:
    virtual ~DeviceEmulator() { }
};
