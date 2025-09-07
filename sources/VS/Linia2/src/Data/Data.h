// 2025/09/07 14:30:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tests/Ranges.h"


// Конвертирует принятое с АЦП значение прямо в амперы
class DataConverterI
{
public:

    DataConverterI(TypeDSet::E t, RangeI::E r) : range{ r }, type_set{ t } { }

    float Convert(int) const;

private:

    RangeI::E     range;
    TypeDSet::E type_set;
};


// Конвертирует принятое с АЦП значение прямо в вольты
class DataConverterU
{
public:

    DataConverterU(TypeDSet::E t, RangeU::E r) : range{ r }, type_set{ t } { }

    float Convert(int) const;

private:

    RangeU::E     range;
    TypeDSet::E type_set;
};
