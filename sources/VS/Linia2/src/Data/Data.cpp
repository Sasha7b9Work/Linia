// 2025/09/07 15:59:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Data/Data.h"


double DataConverter::Convert(int adc) const
{
    double value = k * (double)adc;         // Узнаём абсолютное значение, соответствующее значению АЦП

    return (value + cal.offset) * cal.k;    // И применяем к нему коэффициенты
}


DataConverterI::DataConverterI(TypeDSet::E t, RangeI::E r) :
    DataConverter(t, r),
    range{ r }
{
    // Это значение для данного измерения либо источника приходит с АЦП при максимально возможном значении на входе.
    // Ему соответствует минимальное значение -max_value_ADC
    static const int max_ADC[TypeDSet::Count] =
    {
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
    };

    k = range.MaxValueAbs(RowRange::ForType(type_set)) / (double)max_ADC[type_set];
}


DataConverterU::DataConverterU(TypeDSet::E t, RangeU::E r) :
    DataConverter(t, r),
    range{ r }
{
    // Это значение для данного измерения либо источника приходит с АЦП при максимально возможном значении на входе.
    // Ему соответствует минимальное значение -max_value_ADC
    static const int max_ADC[TypeDSet::Count] =
    {
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
        ((1 << 16) - 1),
    };

    k = range.MaxValueAbs(RowRange::ForType(type_set)) / (double)max_ADC[type_set];
}
