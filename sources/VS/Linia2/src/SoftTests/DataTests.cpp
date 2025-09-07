// 2025/09/07 19:42:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SoftTests/SoftTests.h"
#include "IPPP/Tests/Ranges.h"
#include "Data/Data.h"
#include "Utils/GlobalFunctions.h"


void DataTests::RunAll()
{
    Converter();
}


void DataTests::Converter()
{
    bool correct = true;

    for (TypeDSet t = (TypeDSet::E)0; t.value < TypeDSet::Count; ++t)
    {
        for (RangeI r = (RangeI::E)0; r.value < RangeI::Count; ++r)
        {
            DataConverterI converter{ t.value, r.value };

            RowRange::E row = RowRange::_124;

            double abs = r.MaxValueAbs(row);

            int adc = converter.InverseConvert(abs);

            double abs_2 = converter.Convert(adc);

            if (!GF::ApproxEqual(abs, abs_2))
            {
                LOG_ERROR("%s : %.20f != %.20f, %.20f", r.Name(row), abs, abs_2, abs / abs_2);
                correct = false;
            }
        }

        for (RangeU r = (RangeU::E)0; r.value < RangeU::Count; ++r)
        {
            DataConverterU converter{ t.value, r.value };

            RowRange::E row = RowRange::_124;

            double abs = r.MaxValueAbs(row);

            int adc = converter.InverseConvert(abs);

            double abs_2 = converter.Convert(adc);

            if (!GF::ApproxEqual(abs, abs_2))
            {
                LOG_ERROR("type = %d, %s : %.20f != %.20f, %.20f", (int)t.value, r.Name(row), abs, abs_2, abs / abs_2);
                correct = false;
            }
        }
    }

    if (correct)
    {
        LOG_WRITE("DataTests::Converter() is succseefull");
    }
    else
    {
        LOG_ERROR("DataTests::Converter() is fail");
    }
}
