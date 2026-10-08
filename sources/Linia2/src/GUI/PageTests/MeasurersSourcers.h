// 2026/10/08 11:47:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#pragma once
#include "Utils/AutoBufferedPaintDC.h"
#include "Settings/Tests/SettingsTests.h"
#include "GUI/PageTests/Entities/MeasurerSourcer.h"


// Источники и измерители для всех каналов
struct MeasurersSourcers
{
    void Draw(AutoBufferedPaintDC &, const Chan &, int x);
    void Create();
    void HideAll();
    void Tune();

private:

    MeasurerVoltageCurrent *measBase = nullptr;
    SourceVoltageCurrent *srcVoltageCurrentBase = nullptr;

    MeasurerVoltageCurrent *measSubstrate = nullptr;
    SourceVoltageCurrent *srcVoltageCurrentSubstrate = nullptr;

    Ampermeter *ampCollector = nullptr;
    Voltmeter *voltCollector = nullptr;
    SourceVoltage *srcVoltageCollector = nullptr;

    // Возвращает позицию по Y источника либо измерителя
    int PosY(int);
};
