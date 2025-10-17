// 2025/10/17 11:39:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Chip
{
    // Под этим именем она обозначена в плате контроллера stm32
    wxString GetNameSTM32() const
    {
        return "";
    }
};


struct DAC : public Chip
{
    enum E
    {
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        _6,
        Count
    };
};


struct REG : public Chip
{
    enum E
    {
        _0,
        _1,
        _2,
        _3,
        _4,
        _5,
        Count
    };
};


struct FPGA : public Chip
{
    enum E
    {
        _0,
        _1,
        _2,
        Count
    };
};


extern DAC dacs[DAC::Count];
extern REG regs[REG::Count];
extern FPGA fpgas[FPGA::Count];
