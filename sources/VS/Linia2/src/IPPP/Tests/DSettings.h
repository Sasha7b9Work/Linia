// 2025/09/11 14:47:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "IPPP/Tests/Ranges.h"


struct CalK
{
    double offset;  // Смещение
    double k;       // Коэффициент
};


struct DSettings
{
    CalK calI[TypeDSet::Count][RangeI::Count];
    CalK calU[TypeDSet::Count][RangeU::Count];
};


// Здесь калибровочные коэффициенты
namespace DSet
{
    const CalK &Get(TypeDSet::E, RangeI::E);
    const CalK &Get(TypeDSet::E, RangeU::E);

    void Set(TypeDSet::E, RangeI::E, const CalK &);
    void Set(TypeDSet::E, RangeU::E, const CalK &);
}