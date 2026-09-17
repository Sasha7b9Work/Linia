// 2025/10/24 08:54:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@gmail.com
#include "defines.h"
#include "Device/Sources.h"
#include "Hardware/HAL/HAL_PINS.h"


namespace Source50V
{
    static PinOut pin50EP(Port::_F, Pin::_12);   // pin50
    static PinOut pin50EN(Port::_F, Pin::_11);   // pin49
}


namespace Source3kV
{
    PinOut pinENRGV(Port::_F, Pin::_8);         // pin20 Разрешение регистра источника напряжения 3кВ

    PinOut *GetPinENRGV()
    {
        return &pinENRGV;
    }
}


void Source50V::Init()
{
    pin50EP.Init();
    pin50EP.ToLow();

    pin50EN.Init();
    pin50EP.ToLow();
}


void Source50V::Enable50Plus(bool en)
{
    pin50EP.Set(en);
}


void Source50V::Enable50Minus(bool en)
{
    pin50EP.Set(en);
}


void Source3kV::Init()
{
    pinENRGV.Init();
}
