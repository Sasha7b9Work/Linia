// 2025/09/22 13:39:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device/FPGA.h"
#include "Hardware/HAL/HAL_PINS.h"
#include "Hardware/Timer.h"


namespace FPGA
{
    static PinIn  pinLIMIT(Port::_E, Pin::_7);               // 58
    static PinOut pinA0_RG(Port::_G, Pin::_5);               // 90
    static PinOut pinA1_RG(Port::_G, Pin::_6);               // 91
    static PinOut pinCLK_RG(Port::_G, Pin::_7);              // 92
    static PinOut pinWR_RG(Port::_G, Pin::_8);               // 93
    static PinOut pinDAT_RG(Port::_D, Pin::_13);             // 82
    static PinOut pinSTART_TB(Port::_G, Pin::_4);            // 89

    static uint lengths[10] = { 9, 8, 8, 0, 0, 0, 0, 0, 0, 0 };

    // Дать start FPGA
    static void WriteStart();

    static TimeMeterMS meter;
    static bool is_running_scan = false;    // Если true - идёт развёртка

    static void Pause();
}


void FPGA::Init()
{
    pinLIMIT.Init();
    pinA0_RG.Init();
    pinA1_RG.Init();

    pinCLK_RG.Init();
    pinCLK_RG.ToLow();

    pinWR_RG.Init();
    pinWR_RG.ToLow();

    pinDAT_RG.Init();

    pinSTART_TB.Init();
    pinSTART_TB.ToLow();
}


void FPGA::Update()
{
    Reg::Write(0, 1);

    Timer::DelayMS(1000);

    WriteStart();

    Timer::DelayMS(2000);

    /*
    if (!is_running_scan)
    {
        return;
    }

    if (meter.ElapsedTime() >= 1000)
    {
        WriteStart();
        meter.Reset();
    }
    */
}


void FPGA::StartScan(uint /*periodMS*/)
{
    is_running_scan = true;

    WriteStart();
}


void FPGA::StopScan()
{
    is_running_scan = false;
}


void FPGA::WriteStart()
{
    pinSTART_TB.ToHi();
    Pause();
    pinSTART_TB.ToLow();
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
        Pause();
        pinCLK_RG.ToLow();
    }

    pinWR_RG.ToHi();
    Pause();
    pinWR_RG.ToLow();
}


void FPGA::Pause()
{
    volatile int i = 10000;

    while (i > 0)
    {
        i--;
    }
}
