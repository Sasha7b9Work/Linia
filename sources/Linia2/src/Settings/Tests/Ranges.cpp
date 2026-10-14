// 2025/7/27 22:27:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Settings/Tests/Ranges.h"
#include "Utils/StringUtils.h"
#pragma warning(push, 0)
    #include <wx/arrstr.h>
#pragma warning(pop)


const RangeI::Desc RangeI::desc[RangeI::Count] =
{
    { { "400", "500" },  { "pA", "пА" },  { 4e-10, 5e-10 } },
    { { "1",   "1" },    { "nA", "нА" },  { 1e-9,  1e-9 } },
    { { "2",   "2" },    { "nA", "нА" },  { 2e-9,  2e-9 } },
    { { "4",   "5" },    { "nA", "нА" },  { 4e-9,  5e-9 } },
    { { "10",  "10" },   { "nA", "нА" },  { 1e-8,  1e-8 } },
    { { "20",  "20" },   { "nA", "нА" },  { 2e-8,  2e-8 } },
    { { "40",  "50" },   { "nA", "нА" },  { 4e-8,  5e-8 } },
    { { "100", "100" },  { "nA", "нА" },  { 1e-7,  1e-7 } },
    { { "200", "200" },  { "nA", "нА" },  { 2e-7,  2e-7 } },
    { { "400", "500" },  { "nA", "нА" },  { 4e-7,  4e-7 } },
    { { "1",   "1" },    { "uA", "мкА" }, { 1e-6,  1e-6 } },
    { { "2",   "2" },    { "uA", "мкА" }, { 2e-6,  2e-6 } },
    { { "4",   "5" },    { "uA", "мкА" }, { 4e-6,  5e-6 } },
    { { "10",  "10" },   { "uA", "мкА" }, { 1e-5,  1e-5 } },
    { { "20",  "20" },   { "uA", "мкА" }, { 2e-5,  2e-5 } },
    { { "40",  "50" },   { "uA", "мкА" }, { 4e-5,  5e-5 } },
    { { "100", "100" },  { "uA", "мкА" }, { 1e-4,  1e-4 } },
    { { "200", "200" },  { "uA", "мкА" }, { 2e-4,  2e-4 } },
    { { "400", "500" },  { "uA", "мкА" }, { 4e-4,  5e-4 } },
    { { "1",   "1" },    { "mA", "мА" },  { 1e-3,  1e-3 } },
    { { "2",   "2" },    { "mA", "мА" },  { 2e-3,  2e-3 } },
    { { "4",   "5" },    { "mA", "мА" },  { 4e-3,  5e-3 } },
    { { "10",  "10" },   { "mA", "мА" },  { 1e-2,  1e-2 } },
    { { "20",  "20" },   { "mA", "мА" },  { 2e-2,  2e-2 } },
    { { "40",  "50" },   { "mA", "мА" },  { 4e-2,  5e-2 } },
    { { "100", "100" },  { "mA", "мА" },  { 1e-1,  1e-1 } },
    { { "200", "200" },  { "mA", "мА" },  { 2e-1,  2e-1 } },
    { { "400", "500" },  { "mA", "мА" },  { 4e-1,  5e-1 } },
    { { "1",    "1" },   { "A",  "А" },   { 1e0,   1e0  } },
    { { "2",    "2" },   { "A",  "А" },   { 2e0,   2e0  } },
    { { "4",    "5" },   { "A",  "А" },   { 4e0,   5e0  } },
    { { "10",   "10" },  { "A",  "А" },   { 1e1,   1e0  } },
    { { "20",   "20" },  { "A",  "А" },   { 2e1,   2e1  } },
    { { "40",   "50" },  { "A",  "А" },   { 4e1,   5e1  } },
    { { "100",  "100" }, { "A",  "А" },   { 1e2,   1e2  } },
    { { "200",  "200" }, { "A",  "А" },   { 2e2,   2e2  } },
    { { "500",  "500" }, { "A",  "А" },   { 5e2,   5e2  } }
};


wxString RangeI::Name(RowRange::E row, bool space, bool lang_ru) const
{
    wxString result{ desc[value].values[row] };

    if (space)
    {
        result.Append(" ");
    }

    result.Append(desc[value].units[lang_ru ? 1 : 0]);

    return result;
}


double RangeI::MaxValueAbs(RowRange::E row) const
{
    return desc[value].max_abs[row];
}


const RangeU::Desc RangeU::desc[RangeU::Count] =
{
    { { "1",   "1" },   { "nV", "нВ" },  { 1e-9, 1e-9 } },
    { { "2",   "2" },   { "nV", "нВ" },  { 2e-9, 2e-9 } },
    { { "4",   "5" },   { "nV", "нВ" },  { 4e-9, 5e-9 } },
    { { "10",  "10" },  { "nV", "нВ" },  { 1e-8, 1e-8 } },
    { { "20",  "20" },  { "nV", "нВ" },  { 2e-8, 2e-8 } },
    { { "40",  "50" },  { "nV", "нВ" },  { 4e-8, 5e-8 } },
    { { "100", "100" }, { "nV", "нВ" },  { 1e-7, 1e-7 } },
    { { "200", "200" }, { "nV", "нВ" },  { 2e-7, 2e-7 } },
    { { "400", "500" }, { "nV", "нВ" },  { 4e-7, 5e-7 } },
    { { "1",   "1" },   { "uV", "мкВ" }, { 1e-6, 1e-6 } },
    { { "2",   "2" },   { "uV", "мкВ" }, { 2e-6, 2e-6 } },
    { { "4",   "5" },   { "uV", "мкВ" }, { 4e-6, 5e-6 } },
    { { "10",  "10" },  { "uV", "мкВ" }, { 1e-5, 1e-5 } },
    { { "20",  "20" },  { "uV", "мкВ" }, { 2e-5, 2e-5 } },
    { { "40",  "50" },  { "uV", "мкВ" }, { 4e-5, 5e-5 } },
    { { "100", "100" }, { "uV", "мкВ" }, { 1e-4, 1e-4 } },
    { { "200", "200" }, { "uV", "мкВ" }, { 2e-4, 2e-4 } },
    { { "400", "500" }, { "uV", "мкВ" }, { 4e-4, 5e-4 } },
    { { "1",   "1" },   { "mV", "мВ" },  { 1e-3, 1e-3 } },
    { { "2",   "2" },   { "mV", "мВ" },  { 2e-3, 2e-3 } },
    { { "4",   "5" },   { "mV", "мВ" },  { 4e-3, 5e-3 } },
    { { "10",  "10" },  { "mV", "мВ" },  { 1e-2, 1e-2 } },
    { { "20",  "20" },  { "mV", "мВ" },  { 2e-2, 2e-2 } },
    { { "40",  "50" },  { "mV", "мВ" },  { 4e-2, 5e-2 } },
    { { "100", "100" }, { "mV", "мВ" },  { 1e-1, 1e-1 } },
    { { "200", "200" }, { "mV", "мВ" },  { 2e-1, 2e-1 } },
    { { "400", "500" }, { "mV", "мВ" },  { 4e-1, 5e-1 } },
    { { "1",    "1" },    { "V",  "В" }, { 1e0,  1e0  } },
    { { "2",    "2" },    { "V",  "В" }, { 2e0,  2e0  } },
    { { "4",    "5" },    { "V",  "В" }, { 4e0,  5e0  } },
    { { "10",   "10" },   { "V",  "В" }, { 1e1,  1e1  } },
    { { "20",   "20" },   { "V",  "В" }, { 2e1,  2e1  } },
    { { "40",   "50" },   { "V",  "В" }, { 4e1,  5e1  } },
    { { "100",  "100" },  { "V",  "В" }, { 1e2,  1e2  } },
    { { "200",  "200" },  { "V",  "В" }, { 2e2,  2e2  } },
    { { "400",  "500" },  { "V",  "В" }, { 4e2,  5e2  } },
    { { "1",   "1" },   { "V",  "кВ" },  { 1e3,  1e3  } },
    { { "2",   "2" },   { "V",  "кВ" },  { 2e3,  2e3  } },
    { { "4",   "5" },   { "V",  "кВ" },  { 4e3,  5e3  } },
    { { "10",  "10" },  { "V",  "кВ" },  { 1e4,  1e4  } }
};


wxString RangeU::Name(RowRange::E row, bool space, bool lang_ru) const
{
    wxString result{ desc[value].values[row] };

    if (space)
    {
        result.Append(" ");
    }

    result.Append(desc[value].units[lang_ru ? 1 : 0]);

    return result;
}


double RangeU::MaxValueAbs(RowRange::E row) const
{
    return desc[value].max_abs[row];
}


wxString RangeI::NameStep(RowRange::E row, bool lang_ru) const
{
    wxString name = Name(row);

    wxString val = name.BeforeFirst(' ');

    wxString units = name.AfterFirst(' ');

    int int_value = 0;

    val.ToInt(&int_value);

    if (int_value == 50)
    {
        val = "2.5";
    }
    else if (int_value >= 20)
    {
        val = wxString::Format("%d", int_value / 20);
    }
    else
    {
        val = wxString::Format("%d", int_value * 100 / 2);

        if (units[0] == 'A')        units = 'm' + units;
        else if (units[0] == 'm')   units = "uA";
        else if (units[0] == 'u')   units = "nA";
        else if (units[0] == 'n')   units = "pA";
    }

    if (lang_ru)
    {
        units = SU::ReplaceCurrentUnitsToRU(units);
    }

    return val + " " + units;
}


wxString RangeU::NameStep(RowRange::E row, bool lang_ru) const
{
    wxString name = Name(row, false);

    wxString val = name.BeforeFirst(' ');

    wxString units = name.AfterFirst(' ');

    int int_value = 0;

    val.ToInt(&int_value);

    if (int_value == 50)
    {
        val = "2.5";
    }
    else if (int_value >= 20)
    {
        val = wxString::Format("%d", int_value / 20);
    }
    else
    {
        val = wxString::Format("%d", int_value * 100 / 2);

        if (units[0] == 'V')        units = 'm' + units;
        else if (units[0] == 'k')   units = "V";
        else if (units[0] == 'm')   units = "uV";
        else if (units[0] == 'u')   units = "nV";
        else if (units[0] == 'n')   units = "pV";
    }

    if (lang_ru)
    {
        units = SU::ReplaceVoltageUnitsToRU(units);
    }

    return val + " " + units;
}


RangeI::E RangeI::Min(TypeValue::E type)
{
    struct StructMin
    {
        StructMin(RangeI::E r) : range{r} { }
        RangeI::E range;
    };

    static const StructMin min[TypeValue::Count] =
    {
        _10nA,
        _10nA,
        _40_50nA,
        _40_50nA,
        _40_50nA,
        _40_50nA,
        _40_50nA,
        _40_50nA
    };

    return min[type].range;
}


RangeI::E RangeI::Max(TypeValue::E type)
{
    struct StructMax
    {
        StructMax(RangeI::E r) : range{r} { }
        RangeI::E range;
    };

    static const StructMax max[TypeValue::Count] =
    {
        _40_50A,
        _40_50A,
        _10A,
        _10A,
        _10A,
        _10mA,
        _10mA,
        _10mA
    };

    return max[type].range;
}


RangeU::E RangeU::Min(TypeValue::E type)
{
    struct StructMin
    {
        StructMin(RangeU::E r) : range{r} { }
        RangeU::E range;
    };

    static const StructMin min[TypeValue::Count] =
    {
        _400_500mV,
        _400_500mV,
        _1V,
        _1V,
        _1V,
        _1V,
        _1V,
        _1V
    };

    return min[type].range;
}


RangeU::E RangeU::Max(TypeValue::E type)
{
    struct StructMax
    {
        StructMax(RangeU::E r) : range{r} { }
        RangeU::E range;
    };

    static const StructMax max[TypeValue::Count] =
    {
        _2kV,
        _2kV,
        _40_50V,
        _40_50V,
        _40_50V,
        _40_50V,
        _40_50V,
        _40_50V
    };

    return max[type].range;
}


void RangeI::FillArrayStrings(wxArrayString &arr, TypeValue::E type, bool steps)
{
    arr.clear();
    for (int i = RangeI::Min(type); i <= RangeI::Max(type); i++)
    {
        if (steps)
        {
            arr.push_back(RangeI((RangeI::E)i).NameStep(RowRange(type)));
        }
        else
        {
            arr.push_back(RangeI((RangeI::E)i).Name(RowRange(type)));
        }
    }
}


void RangeU::FillArrayStrings(wxArrayString &arr, TypeValue::E type, bool steps)
{
    arr.clear();

    for (int i = RangeU::Min(type); i <= RangeU::Max(type); i++)
    {
        if (steps)
        {
            arr.push_back(RangeU((RangeU::E)i).NameStep(RowRange(type)));
        }
        else
        {
            arr.push_back(RangeU((RangeU::E)i).Name(RowRange(type)));
        }
    }
}


RowRange::RowRange(TypeValue::E type)
{
    struct StructRow
    {
        StructRow(E r) : row{r} { }
        E row;
    };

    static const StructRow row[TypeValue::Count] =
    {
        _125,
        _125,
        _124,
        _124,
        _124,
        _124,
        _124,
        _124
    };

    value = row[type].row;
}


RowRange::RowRange(Chan::E ch)
{
    static const E r[Chan::Count] =
    {
        _125,
        _124,
        _124,
        _124
    };

    value = r[ch];
}
