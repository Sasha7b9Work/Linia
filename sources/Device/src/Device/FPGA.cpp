// 2025/09/22 13:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/FPGA.h"
#include "Hardware/HAL/HAL_PINS.h"


namespace FPGA
{
    static PinIn  pinLIMIT(Port::_E, Pin::_7);               // 58
    static PinOut pinA0_RG(Port::_G, Pin::_5);               // 90
    static PinOut pinA1_RG(Port::_G, Pin::_6);               // 91
    static PinOut pinCLK_RG(Port::_G, Pin::_7);              // 92
    static PinOut pinWR_RG(Port::_G, Pin::_8);               // 93
    static PinOut pinDAT_RG(Port::_D, Pin::_13);             // 82
    static PinOut pinSTART_TB(Port::_G, Pin::_3);            // 89

    static uint lengths[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}


void FPGA::Init()
{
    pinLIMIT.Init();
    pinA0_RG.Init();
    pinA1_RG.Init();
    pinCLK_RG.Init();
    pinWR_RG.Init();
    pinDAT_RG.Init();
    pinSTART_TB.Init();

    pinCLK_RG.ToLow();
    pinWR_RG.ToLow();
}


void FPGA::StartScan(uint /*periodMS*/)
{

}


void FPGA::StopScan()
{

}


void FPGA::Reg::SetLength(int num, uint length)
{
    lengths[num] = length;
}


void FPGA::Reg::Write(int num, uint value)
{
    pinA0_RG.Set(_GET_BIT(num, 0) != 0);
    pinA1_RG.Set(_GET_BIT(num, 1) != 0);

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
