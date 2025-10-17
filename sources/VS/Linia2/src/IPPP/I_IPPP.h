// 2025/09/07 16:35:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/Tests/Ranges.h"


// Здесь логика работы прибора (логика работы железа фактически)

class I_IPPP
{
public:

    virtual void Init() = 0;

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
    virtual bool ReadData(std::vector<int>(&data)[4]) = 0;

    // Количество точек в режиме DC или импульсном
    virtual void SetPointCount(int) = 0;

    // Схема включения канала
    virtual void SetCircuitConnection(const Chan &, StateJack::E) = 0;

    // Вид развёртки
    virtual void SetTypeScan(TypeScan::E) = 0;

    // Длительность импульса
    // generation_stump - семейство кривых
    virtual void SetPulseDuration(uint durationUS, bool generation_stump) = 0;

    // Первая очередь
    virtual void SetFirstQueue(const Chan &) = 0;
};
