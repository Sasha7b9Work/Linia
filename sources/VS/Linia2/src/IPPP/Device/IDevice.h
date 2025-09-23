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

    virtual void StartMeasurement() = 0;
    virtual void StopMeasurement() = 0;

    // Записать значение в регистр.
    // Если размер регистра меньше 32, записываются младшие биты
    virtual void WriteMicroChip(MicroChip::E, int reg, uint value) = 0;

    // Первая очередь
    virtual void SetFirstQueue(const Chan &) = 0;

    // Длительность импульса
    // generation_stump - семейство кривых
    virtual void SetPulseDuration(uint durationUS, bool generation_stump) = 0;
};
