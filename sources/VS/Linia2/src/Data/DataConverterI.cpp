// 2025/09/07 14:32:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Data.h"


namespace DCI
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


double DataConverterI::Convert(int adc) const
{
    using namespace DCI;

    double k = range.MaxValueAbs(RowRange::ForType(type_set)) / (double)max_ADC[type_set];  // Коэффициент наклона

    double value = k * (double)adc;                                                         // Узнаём абсолютное значение, соответствующее значению АЦП

    const CalK &cal = DSet::Get(type_set, range.value);

    return (value + cal.offset) * cal.k;
}

