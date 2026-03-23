// 2025/09/07 16:35:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/Tests/Ranges.h"


// Здесь логика работы прибора (логика работы железа фактически)

class I_IPPP
{
public:

    static void Create();

    // Вызывается в главном цикле
    virtual void Update() = 0;

    // Вызывается при нажатии кнопки СТАРТ
    virtual void PressButtonStart() = 0;

    // Вызывается при нажатии кнопки СТОП
    virtual void PressButtonStop() = 0;

    static I_IPPP *impl;

protected:

    virtual ~I_IPPP() { }

    // Читает четыре массива значений из ПЛИС
    virtual bool ReadData(
        std::array<int, 100> &data1,
        std::array<int, 100> &data2,
        std::array<int, 100> &data3,
        std::array<int, 100> &data4
    ) = 0;
};
