// 2025/09/07 14:30:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tests/Ranges.h"


// Конвертирует принятое с АЦП значение прямо в амперы
class DataConverterI
{
public:

    DataConverterI(TypeDSet::E, RangeI::E);

    double Convert(int) const;

private:

    RangeI      range;
    TypeDSet::E type_set;
    double      k;          // Коэффициент наклона. Рассчитывается в конструкторе
    const CalK &cal;        // Коэффициенты калибровки устанавливаются в конструкторе
};


// Конвертирует принятое с АЦП значение прямо в вольты
class DataConverterU
{
public:

    DataConverterU(TypeDSet::E, RangeU::E);

    double Convert(int) const;

private:

    RangeU      range;
    TypeDSet::E type_set;
    double      k;          // Коэффициент наклона. Рассчитывается в конструкторе
    const CalK &cal;        // Коэффициенты калибровки устанавливаются в конструкторе
};
