// 2025/10/17 11:39:37 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Chip
{
    virtual ~Chip() { }

    static void Init();

    // Под этим именем она обозначена в плате контроллера stm32
    virtual wxString GetNameSTM32() const = 0;

    // Ширина в битах
    virtual int BitDepth() const = 0;
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

    DAC(E i);

    virtual wxString GetNameSTM32() const override;

    virtual int BitDepth() const override;

private:

    E v = Count;
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

    REG(E i);

    virtual wxString GetNameSTM32() const override;

    virtual int BitDepth() const override;

private:

    E v = Count;
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

    FPGA(E i);

    virtual wxString GetNameSTM32() const override;

    virtual int BitDepth() const override;

private:

    E v = Count;
};


extern DAC *dacs[DAC::Count];
extern REG *regs[REG::Count];
extern FPGA *fpgas[FPGA::Count];
