// 2025/08/29 18:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tests/SettingsTests.h"


class IDeviceTest
{
public:
    virtual void Init() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;

    // Функция, которая будет передавать принятые данные
    virtual void SetCallbackReadData(void (*func)(int16)) = 0;

    virtual void WriteSchemeInclusion(StateJack::E chC, StateJack::E chB, StateJack::E chS) = 0;

    virtual ~IDeviceTest() { }
};


extern IDeviceTest *devicet;
