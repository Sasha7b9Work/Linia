// 2025/7/27 22:17:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct RangeI
{
    enum E
    {
        _1nA,
        _2nA,
        _5nA,
        _10nA,
        _20nA,
        _50nA,
        _100nA,
        _200nA,
        _500nA,
        _1uA,
        _2uA,
        _5uA,
        _10uA,
        _20uA,
        _50uA,
        _100uA,
        _200uA,
        _500uA,
        _1mA,
        _2mA,
        _5mA,
        _10mA,
        _20mA,
        _50mA,
        _100mA,
        _200mA,
        _500mA,
        _1A,
        _2A,
        _5A,
        _10A,
        _20A,
        _50A,
        _100A,
        _200A,
        _500A,
        Count
    };

    E value;

    RangeI(E v) : value(v) { }

    pchar Name() const;
};


struct RangeU
{
    enum E
    {
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
};
