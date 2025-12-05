// 2025/09/22 13:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/FPGA.h"
#include "Hardware/HAL/HAL_PINS.h"


namespace FPGA
{
    static PinIn  pinLIMIT(Port::_E, Pin::_7);               // 58
    static PinOut pinA0_RG(Port::_G, Pin::_5);               // 90
    static PinOut pinA1_RG(Port::_G, Pin::_6);               // 91
    static PinOut pinA2_RG(Port::_G, Pin::_3);               // 89
    static PinOut pinCLK_RG(Port::_G, Pin::_7);              // 92
    static PinOut pinWR_RG(Port::_G, Pin::_8);               // 93
    static PinOut pinDAT_RG(Port::_D, Pin::_13);             // 82    \todo Так ли это?
    static Pin    pinST_TB(Port::_F, Pin::_5, PinMode::In);  // 15    \todo Так ли это?

    static uint lengths[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}


void FPGA::Init()
{
    pinCLK_RG.ToLow();
    pinWR_RG.ToLow();
}


void FPGA::Reg::SetLength(int num, uint length)
{
    lengths[num] = length;
}


void FPGA::Reg::Write(int num, uint value)
{
    pinA0_RG.Set(_GET_BIT(num, 0) != 0);
    pinA1_RG.Set(_GET_BIT(num, 1) != 0);
    pinA2_RG.Set(_GET_BIT(num, 2) != 0);

    int length = (int)lengths[num];

    for (int bit = length - 1; bit >= 0; bit--)
    {
        pinDAT_RG.Set(_GET_BIT(value, bit) != 0);
        pinCLK_RG.ToHi();
        pinCLK_RG.ToLow();
    }

    pinWR_RG.ToHi();
    pinWR_RG.ToLow();
}
