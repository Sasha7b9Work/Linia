// 2025/09/07 14:56:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Data.h"


namespace DCU
{
    // Это значение для данного измерения либо источника приходит с АЦП при максимально возможном значении на входе.
    // Ему соответствует минимальное значение -max_value_ADC
    static int max_ADC[TypeDSet::Count] =
    {
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
    };
}


DataConverterU::DataConverterU(TypeDSet::E t, RangeU::E r) :
    range{ r },
    type_set{ t },
    cal{ DSet::Get(type_set, range.value) }
{
    using namespace DCU;

    k = range.MaxValueAbs(RowRange::ForType(type_set)) / (double)max_ADC[type_set];
}


double DataConverterU::Convert(int adc) const
{
    double value = k * (double)adc;                                                         // Узнаём абсолютное значение, соответствующее значению АЦП

    return (value + cal.offset) * cal.k;
}

