// 2025/08/29 18:02:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class IDeviceTest
{
public:
    virtual void Init() = 0;
    virtual void Start() = 0;
    virtual void Stop() = 0;

    virtual ~IDeviceTest() { }
};


extern IDeviceTest *devicet;
