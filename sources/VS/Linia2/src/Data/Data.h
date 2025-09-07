// 2025/09/07 14:30:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Tests/Ranges.h"


class DataConverterI
{
public:

    DataConverterI(RangeI::E r) : range{r} { }

    float Convert(uint16);
    float Convert(uint);

private:

    RangeI::E range;
};


class DataConverterU
{
public:

    DataConverterU(RangeU::E r) : range{r} { }

    float Convert(uint16);
    float Convert(uint);

private:

    RangeU::E range;
};
