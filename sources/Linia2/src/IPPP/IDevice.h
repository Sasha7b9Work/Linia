#pragma once
#include "Settings/Tests/SettingsTests.h"
#include "Settings/Tests/Ranges.h"


// Здесь идёт обмен с контроллером

class IDevice
{
public:

    static IDevice *impl;

    virtual ~IDevice() = default;

    static IDevice *Create();

    virtual bool Init() = 0;

    virtual void DeInit() = 0;

    virtual void ApplicationTask() = 0;

    virtual bool IsConnected() const = 0;

    // Заслать команду и завершить нулём
    virtual void SendCommand(pchar format, ...) const = 0;

    // Читает четыре массива значений из ПЛИС
    virtual bool ReadData(int data_dac[NUMBER_ADC][POINTS_IN_SAMPLE_ADC], int data_code[POINTS_IN_SAMPLE_ADC]) = 0;
};


/*
    :FPGA[0...2]:LENGTH [1...32]    Установка длины регистра FPGA
    :FPGA[0...2]:WRITE XXXXXh       Запись шестнадцатиричного значения в регистр FPGA
    :SCAN:START  Xms                Запуск развёртки с периодом X ms
    :SCAN:STOP                      Останов развёртки
*/
