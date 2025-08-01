// 2025/7/27 22:17:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace DSet
{
    struct Type
    {
        enum E
        {
            ChanC_Meas,
            ChanB_Meas,
            ChanB_Source,
            ChanB_Limit,
            ChanS_Meas,
            ChanS_Source,
            ChanS_Limit,
            Count
        };

        E value;

        Type(E v) : value(v) {}
    };
}


struct RowI
{
    enum E
    {
        _124,
        _125,
        Count
    };
};


struct RangeI
{
    enum E
    {
        _1nA,
        _2nA,
        _4_5nA,
        _10nA,
        _20nA,
        _40_50nA,
        _100nA,
        _200nA,
        _400_500nA,
        _1uA,
        _2uA,
        _4_5uA,
        _10uA,
        _20uA,
        _40_50uA,
        _100uA,
        _200uA,
        _400_500uA,
        _1mA,
        _2mA,
        _4_5mA,
        _10mA,
        _20mA,
        _40_50mA,
        _100mA,
        _200mA,
        _400_500mA,
        _1A,
        _2A,
        _4_5A,
        _10A,
        _20A,
        _40_50A,
        _100A,
        _200A,
        _400_500A,
        Count
    };

    E value;

    RangeI(E v) : value(v) { }

    pchar Name(RowI::E) const;

    void operator++(int)
    {
        value = (E)(value + 1);
    }

    static E Min(DSet::Type::E);
    static E Max(DSet::Type::E);
};


struct RangeU
{
    enum E
    {
        _1nV,
        _2nV,
        _5nV,
        _10nV,
        _20nV,
        _50nV,
        _100nV,
        _200nV,
        _500nV,
        _1uV,
        _2uV,
        _5uV,
        _10uV,
        _20uV,
        _50uV,
        _100uV,
        _200uV,
        _500uV,
        _1mV,
        _2mV,
        _5mV,
        _10mV,
        _20mV,
        _50mV,
        _100mV,
        _200mV,
        _500mV,
        _1V,
        _2V,
        _5V,
        _10V,
        _20V,
        _50V,
        _100V,
        _200V,
        _500V,
        _1kV,
        _2kV,
        _5kV,
        _10kV,
        Count
    };

    E value;

    RangeU(E v) : value(v) { }

    pchar Name() const;

    void operator++(int)
    {
        value = (E)(value + 1);
    }

    static E Min(DSet::Type::E);
    static E Max(DSet::Type::E);
};


struct CalK
{
    double offset;  // Смещение
    double k;       // Коэффициент
};


namespace DSet
{
    const CalK &Get(Type::E, RangeI::E);
    const CalK &Get(Type::E, RangeU::E);

    void Set(Type::E, RangeI::E, const CalK &);
    void Set(Type::E, RangeU::E, const CalK &);
}
