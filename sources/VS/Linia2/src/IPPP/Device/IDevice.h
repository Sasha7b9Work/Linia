#pragma once
#include "IPPP/Tests/SettingsTests.h"
#include "IPPP/Tests/Ranges.h"


// Здесь идёт обмен с контроллером

class IDevice
{
public:

    static IDevice *impl;

    virtual ~IDevice() = default;

    virtual bool Init() = 0;

    virtual void Shutdown() = 0;

    virtual bool IsConnected() const = 0;
};
